// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentGridWorldSubsystem.h"

#include "EnvironmentGridSettings.h"

void UEnvironmentGridWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();

	_adjacencyListInitialization();


	// TODO: change this so that the cells' spawn location depends on the player spawn location
	ActivateCellsSurroundingLocation(FVector::ZeroVector);
}

void UEnvironmentGridWorldSubsystem::ActivateCellsSurroundingLocation(const FVector location)
{
	const double CellSide = GetDefault<UEnvironmentGridSettings>()->GetCellSide();


	TPair<FCellCoordinates, TSet<FCellCoordinates>> bestCellAndNeighbors;
	double shortestDistance = 0.;

	for (const auto& cellAndNeighbors : _adjacencyList) {
		const auto& cellCoords = cellAndNeighbors.Key;
		const FVector2D cellLocation = FVector2D{ cellCoords.X, cellCoords.Y } * CellSide;

	}
}

void UEnvironmentGridWorldSubsystem::_adjacencyListInitialization()
{
	const UEnvironmentGridSettings* gridSettings = GetDefault<UEnvironmentGridSettings>();
	_adjacencyList.Reserve(gridSettings->GetGridYCells() * gridSettings->GetGridXCells());

	const int32 gridYExtension = gridSettings->GetGridYCells() / 2;
	const int32 gridXExtension = gridSettings->GetGridXCells() / 2;

	for (int32 j = -gridYExtension; j <= gridYExtension; ++j) {
		for (int32 i = gridXExtension; i >= -gridXExtension; --i) {
			FCellCoordinates currentCellCoords{ i, j };
			TSet<FCellCoordinates> neighbors;

			// X
			// X O
			// X
			if (j - 1 > -5) {
				neighbors.Add(FCellCoordinates{ static_cast<double>(i), static_cast<double>(j - 1) });

				if (i + 1 < 5) {
					neighbors.Add(FCellCoordinates{ static_cast<double>(i + 1), static_cast<double>(j - 1) });
				}

				if (i - 1 > -5) {
					neighbors.Add(FCellCoordinates{ static_cast<double>(i - 1), static_cast<double>(j - 1) });
				}
			}

			// X X
			// X O
			// X
			if (i + 1 < 5) {
				neighbors.Add(FCellCoordinates{ static_cast<double>(i + 1), static_cast<double>(j) });
			}

			// X X
			// X O
			// X X
			if (i - 1 > -5) {
				neighbors.Add(FCellCoordinates{ static_cast<double>(i - 1), static_cast<double>(j) });
			}

			// X X X
			// X O X
			// X X X
			if (j + 1 < 5) {
				neighbors.Add(FCellCoordinates{ static_cast<double>(i), static_cast<double>(j + 1) });

				if (i + 1 < 5) {
					neighbors.Add(FCellCoordinates{ static_cast<double>(i + 1), static_cast<double>(j + 1) });
				}

				if (i - 1 > -5) {
					neighbors.Add(FCellCoordinates{ static_cast<double>(i - 1), static_cast<double>(j + 1) });
				}
			}

			_adjacencyList.Emplace(currentCellCoords, MoveTemp(neighbors));
		}
	}
}