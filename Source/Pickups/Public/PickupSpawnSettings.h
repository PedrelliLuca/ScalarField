// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PickupSpawnSettings.generated.h"

/*!
 * \brief Allows us to set up the properties of the pickups spawned by IPickupSpawners  
 */
UCLASS(Config = "Pickups", defaultconfig, meta = (DisplayName = "Pickup Spawn Settings"))
class PICKUPS_API UPickupSpawnSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	TSubclassOf<AActor> GetPickupClass() const { return _pickupClass; }

private:
	UPROPERTY(EditAnywhere, Config, Category = "Pickup", meta = (MustImplement = "Pickup"))
	TSubclassOf<AActor> _pickupClass;
};
