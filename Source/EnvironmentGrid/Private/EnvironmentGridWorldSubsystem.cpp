// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentGridWorldSubsystem.h"

#include "Algo/Copy.h"
#include "EnvironmentCell.h"
#include "EnvironmentCellSettings.h"
#include "EnvironmentGridSettings.h"

void UEnvironmentGridWorldSubsystem::SpawnGrid() {
	const UEnvironmentGridSettings* gridSettings = GetDefault<UEnvironmentGridSettings>();
	_adjacencyList.Reserve(gridSettings->GetGridYCells() * gridSettings->GetGridXCells());

	const double cellSide = GetDefault<UEnvironmentCellSettings>()->GetCellSide();

	const int32 gridYExtension = gridSettings->GetGridYCells() / 2;
	const int32 gridXExtension = gridSettings->GetGridXCells() / 2;

	for (int32 j = -gridYExtension; j <= gridYExtension; ++j) {
		for (int32 i = gridXExtension; i >= -gridXExtension; --i) {
			FCellCoordinates currentCellCoords{ i, j };

			// Spawning the cell
			_spawnCellAtCoordinates(currentCellCoords, cellSide);

			// Setup of the cell's adjacency list
			TSet<FCellCoordinates> neighbors;

			// X
			// X O
			// X
			if (j - 1 > -5) {
				neighbors.Add(FCellCoordinates{ i, j - 1 });

				if (i + 1 < 5) {
					neighbors.Add(FCellCoordinates{ i + 1, j - 1 });
				}

				if (i - 1 > -5) {
					neighbors.Add(FCellCoordinates{ i - 1, j - 1 });
				}
			}

			// X X
			// X O
			// X
			if (i + 1 < 5) {
				neighbors.Add(FCellCoordinates{ i + 1, j });
			}

			// X X
			// X O
			// X X
			if (i - 1 > -5) {
				neighbors.Add(FCellCoordinates{ i - 1, j });
			}

			// X X X
			// X O X
			// X X X
			if (j + 1 < 5) {
				neighbors.Add(FCellCoordinates{ i, j + 1 });

				if (i + 1 < 5) {
					neighbors.Add(FCellCoordinates{ i + 1, j + 1 });
				}

				if (i - 1 > -5) {
					neighbors.Add(FCellCoordinates{ i - 1, j + 1 });
				}
			}

			_adjacencyList.Emplace(currentCellCoords, MoveTemp(neighbors));
		}
	}
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

void UEnvironmentGridWorldSubsystem::_spawnCellAtCoordinates(const FCellCoordinates coordinates, const double cellSide) {
	// Cell spawn
	const FVector cellLocation = FVector{ static_cast<FVector2D>(coordinates), 0.5 } * cellSide;
	TObjectPtr<AEnvironmentCell> currentCell = GetWorld()->SpawnActor<AEnvironmentCell>(cellLocation, FRotator::ZeroRotator);

	// Cells are frozen by default
	currentCell->FreezeTime();

	// Coordinates setup
	currentCell->SetCoordinates(coordinates);

	// Bindings' setup
	currentCell->OnCellBeginningOverlap.AddUObject(this, &UEnvironmentGridWorldSubsystem::_onCellEntered);
	currentCell->OnCellEndingOverlap.AddUObject(this, &UEnvironmentGridWorldSubsystem::_onCellLeft);

	// Cache setup
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