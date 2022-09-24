// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellCoordinates.h"
#include "Subsystems/WorldSubsystem.h"

#include "EnvironmentGridWorldSubsystem.generated.h"

class AEnvironmentCell;

/**
 * 
 */
UCLASS()
class SCALARFIELD_API UEnvironmentGridWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/* Called from maps that have an environment grid */
	UFUNCTION(BlueprintCallable)
	void OnMapStart(FVector playerLocation);

private:
	void _spawnGrid();
	void _activateCellsSurroundingLocation(FVector location);
	void _onCellEntered(FCellCoordinates cellEntered);
	void _onCellLeft(FCellCoordinates cellLeft);

	TMap<FCellCoordinates, TSet<FCellCoordinates>> _adjacencyList;
	TMap<FCellCoordinates, TObjectPtr<AEnvironmentCell>> _cells;
	FCellCoordinates _lastEnteredCell;
};
