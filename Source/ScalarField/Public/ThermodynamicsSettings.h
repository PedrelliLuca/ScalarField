// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "ThermodynamicsSettings.generated.h"

/*!
 * \brief Allows us to set up the geometrical properties of the environment cells and how they're visualized
 */
UCLASS(Config = "Thermodynamics", defaultconfig, meta = (DisplayName = "Thermodynamics Settings"))
class SCALARFIELD_API UThermodynamicsSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    UMaterial* GetThermodynamicsMaterial() const { return _thermodynamicsMaterial.LoadSynchronous(); }

private:
    UPROPERTY(EditAnywhere, Config, Category = "Visualization", meta = (DisplayName = "Material"))
    TSoftObjectPtr<UMaterial> _thermodynamicsMaterial;
};
