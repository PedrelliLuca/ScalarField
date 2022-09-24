// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "EnvironmentCellSettings.generated.h"

/**
 *
 */
UCLASS(Config = "EnvironmentSystem", defaultconfig, meta = (DisplayName = "Environment Cell Settings"))
class SCALARFIELD_API UEnvironmentCellSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	double GetCellSide() const { return _cellSide; }
	UStaticMesh* GetCellPlaceholderMesh() const { return _cellPlaceholderMeshInterface.LoadSynchronous(); }
	UMaterial* GetCellPlaceholderMaterial() const { return _cellPlaceholderMaterialInterface.LoadSynchronous(); }
	FLinearColor GetCellFrozenColor() const { return _cellFrozenColor; }
	FLinearColor GetCellUnfrozenColor() const { return _cellUnfrozenColor; }

private:
	UPROPERTY(Config, EditAnywhere, Category = "Dimensions", meta = (DisplayName = "Side (cm)"))
	double _cellSide = 200.;

	UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Mesh"))
    TSoftObjectPtr<UStaticMesh> _cellPlaceholderMeshInterface;

	UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Material"))
    TSoftObjectPtr<UMaterial> _cellPlaceholderMaterialInterface;

	UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Color - Frozen State"))
    FLinearColor _cellFrozenColor;

	UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Color - Unfrozen State"))
    FLinearColor _cellUnfrozenColor;
};
