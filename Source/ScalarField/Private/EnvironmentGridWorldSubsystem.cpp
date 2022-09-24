// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentGridWorldSubsystem.h"

void UEnvironmentGridWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();

	_adjacencyListInitialization();


	// TODO: change this so that the cells 
	ActivateCellsSurroundingLocation(FVector::ZeroVector);
}

void UEnvironmentGridWorldSubsystem::ActivateCellsSurroundingLocation(FVector location)
{
	TPair<FCellCoordinates, TSet<FCellCoordinates>> bestCellAndNeighbors;
	double shortestDistance = 0.;

	for (const auto& cellAndNeighbors : _adjacencyList) {
		const auto& cellCoords = bestCellAndNeighbors.Key;


	}
}

void UEnvironmentGridWorldSubsystem::_adjacencyListInitialization()
{
	_adjacencyList.Reserve(N_CELLS);

	for (int32 j = -4; j < 5; ++j) {
		for (int32 i = 4; i > -5; --i) {
			FCellCoordinates currentCellCoords{ i, j };
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