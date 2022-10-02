// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvironmentGridWorldSubsystem.h"

#include "Algo/Copy.h"
#include "EnvironmentCell.h"
#include "EnvironmentCellSettings.h"

void UEnvironmentGridWorldSubsystem::SpawnGrid(FGridSpawnAttributes gridAttributes) {

	_adjacencyList.Reserve(gridAttributes.NCellsY * gridAttributes.NCellsY);

	const int32 smallestYCoord = -gridAttributes.NCellsY / 2;
	const int32 biggestYCoord = gridAttributes.NCellsY % 2 != 0 ? -smallestYCoord : -smallestYCoord - 1;

	const int32 biggestXCoord = gridAttributes.NCellsX / 2;
	const int32 smallestXCoord = gridAttributes.NCellsX % 2 != 0 ? -biggestXCoord : -biggestXCoord + 1;

	const FVector diagonalCorrection = FVector{ -gridAttributes.Step, gridAttributes.Step, 0. } *0.5;

	for (int32 j = smallestYCoord; j <= biggestYCoord; ++j) {
		for (int32 i = biggestXCoord; i >= smallestXCoord; --i) {
			FCellCoordinates currentCellCoords{ i, j };

			// Spawning the cell
			_spawnCellAtCoordinates(currentCellCoords, gridAttributes.GridCenter, gridAttributes.Step, diagonalCorrection);

			// Setup of the cell's adjacency list
			TSet<FCellCoordinates> neighbors;

			// X
			// X O
			// X
			if (j - 1 >= smallestYCoord) {
				neighbors.Add(FCellCoordinates{ i, j - 1 });

				if (i + 1 <= biggestXCoord) {
					neighbors.Add(FCellCoordinates{ i + 1, j - 1 });
				}

				if (i - 1 >= smallestXCoord) {
					neighbors.Add(FCellCoordinates{ i - 1, j - 1 });
				}
			}

			// X X
			// X O
			// X
			if (i + 1 <= biggestXCoord) {
				neighbors.Add(FCellCoordinates{ i + 1, j });
			}

			// X X
			// X O
			// X X
			if (i - 1 >= smallestXCoord) {
				neighbors.Add(FCellCoordinates{ i - 1, j });
			}

			// X X X
			// X O X
			// X X X
			if (j + 1 <= biggestYCoord) {
				neighbors.Add(FCellCoordinates{ i, j + 1 });

				if (i + 1 <= biggestXCoord) {
					neighbors.Add(FCellCoordinates{ i + 1, j + 1 });
				}

				if (i - 1 >= smallestXCoord) {
					neighbors.Add(FCellCoordinates{ i - 1, j + 1 });
				}
			}

			_adjacencyList.Emplace(currentCellCoords, MoveTemp(neighbors));
		}
	}
}

TMap<FCellCoordinates, TWeakObjectPtr<const AEnvironmentCell>> UEnvironmentGridWorldSubsystem::GetGrid() const {
	TMap<FCellCoordinates, TWeakObjectPtr<const AEnvironmentCell>> constCells;
	constCells.Reserve(_cells.Num());

	Algo::Transform(_cells, constCells, [](const auto& coordCellPair) {
		return TPair<FCellCoordinates, TWeakObjectPtr<const AEnvironmentCell>>{coordCellPair.Key, coordCellPair.Value};
		});

	return constCells;
}

void UEnvironmentGridWorldSubsystem::ActivateOverlappedCells(const TSet<AEnvironmentCell*>& cellsToActivate) {
	for (AEnvironmentCell* const cell : cellsToActivate) {
		// Unfreezing the given cell
		auto coords = cell->GetCoordinates();
		_overlappedCells.Emplace(coords);
		cell->UnfreezeTime();

		// Unfreezing its neighbors
		const auto* const neighborsOfCell = _adjacencyList.Find(coords);
		check(neighborsOfCell != nullptr);
		for (const auto& neighborCoords : *neighborsOfCell) {
			check(_cells.Contains(neighborCoords));
			_cells[neighborCoords]->UnfreezeTime();
		}
	}
}

void UEnvironmentGridWorldSubsystem::_spawnCellAtCoordinates(const FCellCoordinates coordinates, const FVector2D gridCenter, const double step, const FVector diagonalCorrection) {
	// Cell spawn
	const FVector cellLocation = FVector{ gridCenter, 0. } + FVector{ static_cast<FVector2D>(coordinates), 0.5 } *step + diagonalCorrection;
	const FTransform cellTransform{ cellLocation };
	TObjectPtr<AEnvironmentCell> currentCell = GetWorld()->SpawnActorDeferred<AEnvironmentCell>(AEnvironmentCell::StaticClass(), cellTransform);

	// Cells are frozen by default
	currentCell->FreezeTime();
	currentCell->SetSide(step);
	currentCell->SetCoordinates(coordinates);

	// Bindings' setup
	currentCell->OnCellBeginningOverlap.AddUObject(this, &UEnvironmentGridWorldSubsystem::_onCellEntered);
	currentCell->OnCellEndingOverlap.AddUObject(this, &UEnvironmentGridWorldSubsystem::_onCellLeft);

	currentCell->FinishSpawning(cellTransform);

	_cells.Emplace(coordinates, MoveTemp(currentCell));
}

void UEnvironmentGridWorldSubsystem::_onCellEntered(FCellCoordinates cellEntered) {
	_overlappedCells.Emplace(cellEntered);
	const auto& neighborsOfEntered = _adjacencyList[cellEntered];
	for (const auto& neighborCoords : neighborsOfEntered) {
		_cells[neighborCoords]->UnfreezeTime();
	}
}

void UEnvironmentGridWorldSubsystem::_onCellLeft(FCellCoordinates cellLeft) {
	_overlappedCells.Remove(cellLeft);
	const auto& neighborsOfLeft = _adjacencyList[cellLeft];

	// Retrieve all cells that are currently overlapped and their neighbors
	TSet<FCellCoordinates> overlappedAndNeighbors = _overlappedCells;
	for (const auto& overlappedCell : _overlappedCells) {
		overlappedAndNeighbors.Append(_adjacencyList[overlappedCell]);
	}

	// Determine the left cell neighbors that aren't among the set retrieved above
	TSet<FCellCoordinates> uncommonNeighbors;
	Algo::CopyIf(neighborsOfLeft, uncommonNeighbors, [&overlappedAndNeighbors](const FCellCoordinates& neighborOfLeft) {
		return overlappedAndNeighbors.Find(neighborOfLeft) == nullptr;
		});

	// Freezing
	for (const auto& neighborCoords : uncommonNeighbors) {
		_cells[neighborCoords]->FreezeTime();
	}
}