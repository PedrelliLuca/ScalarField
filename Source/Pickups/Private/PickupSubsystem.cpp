// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupSubsystem.h"

void UPickupSubsystem::Initialize(FSubsystemCollectionBase& collection) {
	Super::Initialize(collection);

	_pickupSpawnController = NewObject<UPickupSpawnController>(this, UPickupSpawnController::StaticClass());
}

void UPickupSubsystem::RegisterItemDropper(TScriptInterface<IItemDropper> pickupSpawner) {
	_pickupSpawnController->RegisterItemDropper(MoveTemp(pickupSpawner));
}
