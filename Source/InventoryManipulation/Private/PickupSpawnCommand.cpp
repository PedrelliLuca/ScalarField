// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupSpawnCommand.h"

#include "PickupInterface.h"

void UPickupSpawnCommand::Execute() {
    int32 const droppedQuantity = _inventory->ConsumeItem(_item.Get(), _quantity);

    FActorSpawnParameters spawnParams{};
    spawnParams.bNoFail = true;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FTransform const spawnTransform = _getPickupLocationCallback();

    check(IsValid(_pickupClass));

    TWeakInterfacePtr<IPickup> const pickup = GetWorld()->SpawnActor<AActor>(_pickupClass, spawnTransform, spawnParams);
    check(pickup.IsValid());

    pickup->InitializePickup(_item.GetObject()->GetClass(), droppedQuantity);
}
