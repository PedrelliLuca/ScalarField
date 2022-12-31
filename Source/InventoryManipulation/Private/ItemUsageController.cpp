// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUsageController.h"

#include "TacticalPauseWorldSubsystem.h"

void UItemUsageController::SetItemUsageNotifier(TWeakInterfacePtr<IItemInventoryWidget> itemUsageNotifier) {
	if (_itemUsageNotifier.IsValid()) {
		_itemUsageNotifier->OnItemFromInventoryBeingUsed().Remove(_itemUsageHandle);
		_itemUsageHandle.Reset();
	}
	
	check(itemUsageNotifier.IsValid());
	_itemUsageNotifier = MoveTemp(itemUsageNotifier);
}

void UItemUsageController::BindItemUsage() {
	check(!_itemUsageHandle.IsValid() && _itemUsageNotifier.IsValid());
	_itemUsageHandle = _itemUsageNotifier->OnItemFromInventoryBeingUsed().AddUObject(this, &UItemUsageController::_useItemOfInventory);
}

void UItemUsageController::UnbindItemUsage() {
	check(_itemUsageNotifier.IsValid());
	_itemUsageNotifier->OnItemFromInventoryBeingUsed().Remove(_itemUsageHandle);
	_itemUsageHandle.Reset();
}

void UItemUsageController::_useItemOfInventory(TWeakInterfacePtr<IItem> item, TWeakInterfacePtr<IInventory> inventory) {
	check(item.IsValid() && inventory.IsValid());
	const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
	pauseSubsys->OnTacticalPauseToggle().Remove(_itemUsageOnPauseToggleHandle);
	_itemUsageOnPauseToggleHandle.Reset();
	
	if (!pauseSubsys->IsTacticalPauseOn()) {
		// We are using the item from the inventory => we expect that the inventory contains the item
		check(inventory->FindItemByClass(item.GetObject()->GetClass()).IsValid());

		item->Use(inventory->GetInventoryOwner());
		
		return;
	}

	// The tactical pause is one, we can't use the item yet. We predispose the following lambda to be called once the
	// tactical pause is toggled off.
	
	_itemUsageOnPauseToggleHandle = pauseSubsys->OnTacticalPauseToggle().AddLambda([this, item, inventory](const bool bIsTacticalPauseOn, const double currentWorldTimeDilation) {
		// This must have been called when the tactical pause has been turned off
		check(!bIsTacticalPauseOn);

		// I don't see how the controller could not be valid, its outer is the subsystem...
		check(IsValid(this));

		// We were in tactical pause when this lambda was created, meaning that from the creation and call we do not
		// expect anything to be changed on the inventory owner, whoever it is. Therefore, its inventory and the item
		// in it must still be valid.
		check(item.IsValid())
		check(inventory.IsValid());
		
		_useItemOfInventory(item, inventory);
	});
}
