// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSpawnCommandFactory.h"

#include "PickupSpawnCommand.h"

TScriptInterface<IPauseCommand> UPickupSpawnCommandFactory::CreateCommand() {
	const auto command = NewObject<UPickupSpawnCommand>(this, UPickupSpawnCommand::StaticClass());
	command->_item = MoveTemp(_item);
	command->_inventory = MoveTemp(_inventory);
	command->_quantity = _quantity;
	_quantity = 0;
	command->_getPickupLocationCallback = _getPickupLocationCallback;
	command->_pickupClass = _pickupClass;

	return command;
}

void UPickupSpawnCommandFactory::SetCommandItem(TWeakInterfacePtr<IItem> item) {
	check(item.IsValid());
	_item = MoveTemp(item);
}

void UPickupSpawnCommandFactory::SetCommandInventory(TWeakInterfacePtr<IInventory> inventory) {
	check(inventory.IsValid());
	_inventory = MoveTemp(inventory);
}

void UPickupSpawnCommandFactory::SetCommandQuantity(int32 quantity) {
	check(quantity > 0);
	_quantity = quantity;
}

void UPickupSpawnCommandFactory::SetPickupClass(TSubclassOf<AActor> pickupClass) {
	check(IsValid(pickupClass));
	_pickupClass = MoveTemp(pickupClass);
}

void UPickupSpawnCommandFactory::SetPickupLocationCallback(TFunction<FTransform()> pickupLocationCallback) {
	pickupLocationCallback.CheckCallable();
	_getPickupLocationCallback = MoveTemp(pickupLocationCallback);
}