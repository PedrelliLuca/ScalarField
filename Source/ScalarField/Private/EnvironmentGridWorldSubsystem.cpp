// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentGridWorldSubsystem.h"

#include "Algo/Copy.h"
#include "EnvironmentCell.h"
#include "EnvironmentCellSettings.h"
#include "EnvironmentGridSettings.h"

void UEnvironmentGridWorldSubsystem::OnMapStart(const FVector playerLocation) {
	_spawnGrid();

	_activateCellsSurroundingLocation(playerLocation);
}

void UEnvironmentGridWorldSubsystem::_spawnGrid() {
	const UEnvironmentGridSettings* gridSettings = GetDefault<UEnvironmentGridSettings>();
	_adjacencyList.Reserve(gridSettings->GetGridYCells() * gridSettings->GetGridXCells());

	const double CellSide = GetDefault<UEnvironmentCellSettings>()->GetCellSide();

	const int32 gridYExtension = gridSettings->GetGridYCells() / 2;
	const int32 gridXExtension = gridSettings->GetGridXCells() / 2;

	for (int32 j = -gridYExtension; j <= gridYExtension; ++j) {
		for (int32 i = gridXExtension; i >= -gridXExtension; --i) {
			FCellCoordinates currentCellCoords{ i, j };

			// Cell creation
			const FVector cellLocation = FVector{ static_cast<FVector2D>(currentCellCoords), 0.5 } *CellSide;
			TObjectPtr<AEnvironmentCell> currentCell = GetWorld()->SpawnActor<AEnvironmentCell>(cellLocation, FRotator::ZeroRotator);
			currentCell->FreezeTime();
			currentCell->SetCoordinates(currentCellCoords);
			currentCell->OnCellBeginningOverlap.AddUObject(this, &UEnvironmentGridWorldSubsystem::_onCellEntered);
			currentCell->OnCellEndingOverlap.AddUObject(this, &UEnvironmentGridWorldSubsystem::_onCellLeft);
			_cells.Emplace(currentCellCoords, MoveTemp(currentCell));

			// Neighbors setup
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

void UEnvironmentGridWorldSubsystem::_activateCellsSurroundingLocation(const FVector location) {
	const double CellSide = GetDefault<UEnvironmentCellSettings>()->GetCellSide();

	double shortestDistance = TNumericLimits<double>::Max();
	const FCellCoordinates* closestCoords = nullptr;
	const TSet<FCellCoordinates>* neighborsOfClosestCell = nullptr;

	const FVector2D location2D{ location };
	for (const auto& cellAndNeighbors : _adjacencyList) {
		const auto& cellCoords = cellAndNeighbors.Key;
		const FVector2D cellLocation = static_cast<FVector2D>(cellCoords) * CellSide;

		const double cellDistanceFromLocation = FVector2D::Distance(location2D, cellLocation);
		if (cellDistanceFromLocation < shortestDistance) {
			shortestDistance = cellDistanceFromLocation;
			closestCoords = &cellCoords;
			neighborsOfClosestCell = &cellAndNeighbors.Value;
		}
	}

	check(closestCoords != nullptr && _cells.Contains(*closestCoords));

	_cells[*closestCoords]->UnfreezeTime();

	check(neighborsOfClosestCell != nullptr);
	for (const auto& neighborCoords : *neighborsOfClosestCell) {
		check(_cells.Contains(neighborCoords));
		_cells[neighborCoords]->UnfreezeTime();
	}
}

void UEnvironmentGridWorldSubsystem::_onCellEntered(FCellCoordinates cellEntered) {
	_lastEnteredCell = cellEntered;
	const auto& neighborsOfEntered = _adjacencyList[cellEntered];
	for (const auto& neighborCoords : neighborsOfEntered) {
		if (_cells[neighborCoords]->IsFrozen()) {
			_cells[neighborCoords]->UnfreezeTime();
		}
	}
}

void UEnvironmentGridWorldSubsystem::_onCellLeft(FCellCoordinates cellLeft) {
	const auto& neighborsOfLeft = _adjacencyList[cellLeft];
	const auto& neighborsOfCurrent = _adjacencyList[_lastEnteredCell];

	TSet<FCellCoordinates> uncommonNeighbors;
	Algo::CopyIf(neighborsOfLeft, uncommonNeighbors, [&neighborsOfCurrent, lastEnteredCell = _lastEnteredCell](const FCellCoordinates& neighborOfLeft) {
		return neighborOfLeft != lastEnteredCell && neighborsOfCurrent.Find(neighborOfLeft) == nullptr;
	});

	for (const auto& neighborCoords : uncommonNeighbors) {
		_cells[neighborCoords]->FreezeTime();
	}
}