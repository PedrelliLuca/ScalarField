// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSpawnController.h"

#include "InventoryComponent.h"
#include "PickupInterface.h"
#include "PickupSpawnSettings.h"

UPickupSpawnController::UPickupSpawnController() {
	const auto pickupSpawnSettings = GetDefault<UPickupSpawnSettings>();
	_pickupClass = pickupSpawnSettings->GetPickupClass();
}

void UPickupSpawnController::RegisterItemDropper(TScriptInterface<IItemDropper> itemDropper) {
	itemDropper->OnItemDroppedFromInventory().AddUObject(this, &UPickupSpawnController::_spawnPickup);
}

void UPickupSpawnController::_spawnPickup(const TWeakObjectPtr<UInventoryItem> item, const int32 quantityToDrop, const TWeakObjectPtr<UInventoryComponent> inventory) {
	const int32 droppedQuantity = inventory->ConsumeItem(item.Get(), quantityToDrop);

	FActorSpawnParameters spawnParams{};
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FTransform spawnTransform = inventory->GetOwner()->GetActorTransform();

	check(IsValid(_pickupClass));

	const TScriptInterface<IPickup> pickup = GetWorld()->SpawnActor<AActor>(_pickupClass, spawnTransform, spawnParams);
	check(IsValid(pickup.GetObject()));

	// const auto pickup = Cast<IPickup>(pickupAsActor);
	
	pickup->InitializePickup(item->GetClass(), droppedQuantity);
}
