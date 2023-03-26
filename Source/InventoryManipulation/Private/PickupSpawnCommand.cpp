// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupSpawnCommand.h"

#include "PickupInterface.h"

void UPickupSpawnCommand::Execute() {
    const int32 droppedQuantity = _inventory->ConsumeItem(_item.Get(), _quantity);

    FActorSpawnParameters spawnParams{};
    spawnParams.bNoFail = true;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    const FTransform spawnTransform = _getPickupLocationCallback();

    check(IsValid(_pickupClass));

    const TWeakInterfacePtr<IPickup> pickup = GetWorld()->SpawnActor<AActor>(_pickupClass, spawnTransform, spawnParams);
    check(pickup.IsValid());

    pickup->InitializePickup(_item.GetObject()->GetClass(), droppedQuantity);
}
