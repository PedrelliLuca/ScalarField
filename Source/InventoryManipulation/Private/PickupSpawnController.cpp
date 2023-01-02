// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSpawnController.h"

#include "PickupInterface.h"
#include "PickupSpawnSettings.h"
#include "TacticalPauseWorldSubsystem.h"

UPickupSpawnController::UPickupSpawnController() {
	const auto pickupSpawnSettings = GetDefault<UPickupSpawnSettings>();
	_pickupClass = pickupSpawnSettings->GetPickupClass();
	check(IsValid(_pickupClass));

	_pickupSpawnCmdFactory = CreateDefaultSubobject<UPickupSpawnCommandFactory>(TEXT("Pickup Spawn Command Factory"));
	_pickupSpawnCmdFactory->SetPickupClass(_pickupClass);
}

void UPickupSpawnController::SetItemDropNotifier(TWeakInterfacePtr<IItemInventoryWidget> itemDropNotifier) {
	if (_itemDropNotifier.IsValid()) {
		_itemDropNotifier->OnItemFromInventoryBeingUsed().Remove(_itemDropHandle);
		_itemDropHandle.Reset();
	}
	
	check(itemDropNotifier.IsValid());
	_itemDropNotifier = MoveTemp(itemDropNotifier);
}

void UPickupSpawnController::SetPickupSpawnCallback(TFunction<FTransform()>&& pickupSpawnCallback) {
	pickupSpawnCallback.CheckCallable();
	_getPickupSpawnLocation = MoveTemp(pickupSpawnCallback);
	_pickupSpawnCmdFactory->SetPickupLocationCallback(_getPickupSpawnLocation);
}

void UPickupSpawnController::BindPickupSpawn() {
	check(!_itemDropHandle.IsValid() && _itemDropNotifier.IsValid());
	_itemDropHandle = _itemDropNotifier->OnItemFromInventoryBeingDropped().AddUObject(this, &UPickupSpawnController::_spawnPickup);
}

void UPickupSpawnController::UnbindPickupSpawn() {
	check(_itemDropNotifier.IsValid());
	_itemDropNotifier->OnItemFromInventoryBeingDropped().Remove(_itemDropHandle);
	_itemDropHandle.Reset();
}

void UPickupSpawnController::_spawnPickup(TWeakInterfacePtr<IItem> item, const int32 quantity, TWeakInterfacePtr<IInventory> inventory) {
	check(item.IsValid() && inventory.IsValid());
	const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
	if (!pauseSubsys->IsTacticalPauseOn()) {
		const int32 droppedQuantity = inventory->ConsumeItem(item.Get(), quantity);

		FActorSpawnParameters spawnParams{};
		spawnParams.bNoFail = true;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FTransform spawnTransform = _getPickupSpawnLocation();

		check(IsValid(_pickupClass));

		const TWeakInterfacePtr<IPickup> pickup = GetWorld()->SpawnActor<AActor>(_pickupClass, spawnTransform, spawnParams);
		check(pickup.IsValid());

		pickup->InitializePickup(item.GetObject()->GetClass(), droppedQuantity);
	}
	
	check(IsValid(_pickupSpawnCmdFactory));
	_pickupSpawnCmdFactory->SetCommandItem(MoveTemp(item));
	_pickupSpawnCmdFactory->SetCommandInventory(MoveTemp(inventory));
	_pickupSpawnCmdFactory->SetCommandQuantity(quantity);
	pauseSubsys->SetPauseOffCommand(_pickupSpawnCmdFactory->CreateCommand());
}


