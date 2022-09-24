// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "EnvironmentGridSettings.generated.h"

/**
 *
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Environment Grid Settings"))
class SCALARFIELD_API UEnvironmentGridSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	double GetCellSide() const { return _cellSide; }

	uint32 GetGridYCells() const { return _gridYCells; }
	uint32 GetGridXCells() const { return _gridXCells; }

private:
	UPROPERTY(Config, EditAnywhere, Category = "Cell")
	double _cellSide = 200.;

	UPROPERTY(Config, EditAnywhere, Category = "Grid")
	uint32 _gridYCells = 9;
	UPROPERTY(Config, EditAnywhere, Category = "Grid")
	uint32 _gridXCells = 9;
};
