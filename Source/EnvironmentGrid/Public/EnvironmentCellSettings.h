// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "EnvironmentCellSettings.generated.h"

/*!
 * \brief Allows us to set up the geometrical properties of the environment cells and how they're visualized
 */
UCLASS(Config = "EnvironmentSystem", defaultconfig, meta = (DisplayName = "Environment Cell Settings"))
class ENVIRONMENTGRID_API UEnvironmentCellSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    UStaticMesh* GetCellPlaceholderMesh() const { return _cellPlaceholderMeshInterface.LoadSynchronous(); }
    UMaterial* GetCellPlaceholderMaterial() const { return _cellPlaceholderMaterialInterface.LoadSynchronous(); }
    FLinearColor GetCellFrozenColor() const { return _cellFrozenColor; }
    FLinearColor GetCellUnfrozenColor() const { return _cellUnfrozenColor; }

private:
    UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Mesh"))
    TSoftObjectPtr<UStaticMesh> _cellPlaceholderMeshInterface;

    UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Material"))
    TSoftObjectPtr<UMaterial> _cellPlaceholderMaterialInterface;

    UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Color - Frozen State"))
    FLinearColor _cellFrozenColor;

    UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Color - Unfrozen State"))
    FLinearColor _cellUnfrozenColor;
};
