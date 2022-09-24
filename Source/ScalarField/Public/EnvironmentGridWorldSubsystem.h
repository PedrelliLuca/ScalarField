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
	void PostInitialize() override;

	void ActivateCellsSurroundingLocation(FVector location);


private:
	void _adjacencyListInitialization();

	TMap<FCellCoordinates, TSet<FCellCoordinates>> _adjacencyList;
	TMap<FCellCoordinates, TObjectPtr<AEnvironmentCell>> _cells;
};
