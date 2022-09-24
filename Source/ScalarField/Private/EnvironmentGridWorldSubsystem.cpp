// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentGridWorldSubsystem.h"

#include "EnvironmentCell.h"
#include "EnvironmentGridSettings.h"

void UEnvironmentGridWorldSubsystem::PostInitialize() {
	Super::PostInitialize();

	_allocateGrid();

	// TODO: change this so that the cells' spawn location depends on the player spawn location
	_initializeGridByLocation(FVector::ZeroVector);
}

void UEnvironmentGridWorldSubsystem::_initializeGridByLocation(const FVector location) {
	const double CellSide = GetDefault<UEnvironmentGridSettings>()->GetCellSide();

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

void UEnvironmentGridWorldSubsystem::_allocateGrid() {
	const UEnvironmentGridSettings* gridSettings = GetDefault<UEnvironmentGridSettings>();
	_adjacencyList.Reserve(gridSettings->GetGridYCells() * gridSettings->GetGridXCells());

	const int32 gridYExtension = gridSettings->GetGridYCells() / 2;
	const int32 gridXExtension = gridSettings->GetGridXCells() / 2;

	for (int32 j = -gridYExtension; j <= gridYExtension; ++j) {
		for (int32 i = gridXExtension; i >= -gridXExtension; --i) {
			FCellCoordinates currentCellCoords{ i, j };

			// Cell creation
			TObjectPtr<AEnvironmentCell> currentCell = NewObject<AEnvironmentCell>(this, FName{ currentCellCoords.ToString() });
			currentCell->FreezeTime();
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