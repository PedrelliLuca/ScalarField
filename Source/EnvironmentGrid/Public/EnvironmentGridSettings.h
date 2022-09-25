// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "EnvironmentGridSettings.generated.h"

/*!
 * \brief Allows us to setup the properties of the grid settings
 */
UCLASS(Config = "EnvironmentSystem", defaultconfig, meta = (DisplayName = "Environment Grid Settings"))
class ENVIRONMENTGRID_API UEnvironmentGridSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	uint32 GetGridYCells() const { return _gridYCells; }
	uint32 GetGridXCells() const { return _gridXCells; }

private:
	UPROPERTY(Config, EditAnywhere, Category = "Dimensions")
	uint32 _gridYCells = 9;
	UPROPERTY(Config, EditAnywhere, Category = "Dimensions")
	uint32 _gridXCells = 9;
};
