// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellCoordinates.h"
#include "Subsystems/WorldSubsystem.h"

#include "EnvironmentGridWorldSubsystem.generated.h"

class AEnvironmentCell;

/*!
 * \brief Spawns and manages all environment cells in the world by (un)freezing them depending on the player's location
 */
UCLASS()
class ENVIRONMENTGRID_API UEnvironmentGridWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	struct FGridSpawnAttributes {
		FVector2D GridCenter = FVector2D::ZeroVector;
		double ZExtension = 0.;
		double Step = 0.;
		int32 NCellsX = 0;
		int32 NCellsY = 0;
	};

	/*!
	* \brief Spawns a grid of environment cells in the current map.
	*/
	void SpawnGrid(FGridSpawnAttributes gridAttributes);

	TMap<FCellCoordinates, TWeakObjectPtr<const AEnvironmentCell>> GetGrid() const;
	
	/*!
	* \brief Given a set of environment cells that are being overlapped, activates them and their neighbors
	*/
	void ActivateOverlappedCells(const TSet<AEnvironmentCell*>& cellsToActivate);

private:
	/*!
	* \brief Employs the given coordinates and side to spawn a cell via SpawnActor
	*/
	void _spawnCellAtCoordinates(FCellCoordinates coordinates, FVector2D gridCenter, double side, double z, FVector diagonalCorrection);

	/*!
	* \brief Unfreeze all neighbors of the given cell. Hypothesis: the given cell is unfrozen.
	*/
	void _onCellEntered(FCellCoordinates cellEntered);

	/*!
	* \brief Freezes all neighbors of the given cell that aren't neighbors of overlapped cells. Hypothesis: the given cell is unfrozen.
	*/
	void _onCellLeft(FCellCoordinates cellLeft);

	/*! 
	* \brief Data structure to quickly retrieve the neighbors of any cell 
	*/
	TMap<FCellCoordinates, TSet<FCellCoordinates>> _adjacencyList;

	/*! 
	* \brief Cache-like structure where we keep track of every cell currently on the map 
	*/
	TMap<FCellCoordinates, TObjectPtr<AEnvironmentCell>> _cells;

	/*! 
	* \brief Here we keep the cells that are currently being overlapped by the player's character
	*/
	TSet<FCellCoordinates> _overlappedCells;
};
