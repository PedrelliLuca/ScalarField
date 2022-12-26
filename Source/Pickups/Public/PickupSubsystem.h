// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupSpawnController.h"
#include "ItemDropperInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PickupSubsystem.generated.h"

/*!
 * \brief System that allows to perform operations on pickups. Acts as a facade for pickup controllers.
 */
UCLASS()
class PICKUPS_API UPickupSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& collection) override;

	void RegisterItemDropper(TScriptInterface<IItemDropper> pickupSpawner);

private:
	UPROPERTY()
	TObjectPtr<UPickupSpawnController> _pickupSpawnController{};
};