// Fill out your copyright notice in the Description page of Project Settings.


#include "NewInventoryWidget.h"

void UNewInventoryWidget::SetInventory(TWeakInterfacePtr<IInventory> inventory) {
	if (_inventory.IsValid()) {
		_inventory->OnInventoryUpdated().RemoveAll(this);
	}

	check(inventory.IsValid());
	_inventory = MoveTemp(inventory);
	_inventory->OnInventoryUpdated().AddUObject(this, &UNewInventoryWidget::_resetInventoryItems);

	_resetInventoryItems();
}

void UNewInventoryWidget::_resetInventoryItems() {
	check(IsValid(_itemWidgetClass));
	_inventoryItemsBox->ClearChildren();

	for (const auto& item : _inventory->GetItems()) {
		const auto itemWidget = CreateWidget<UNewInventoryItemWidget>(GetOwningPlayer(), _itemWidgetClass);
		itemWidget->SetItem(item);
		itemWidget->OnItemUsage().AddUObject(this, &UNewInventoryWidget::_onItemBeingUsed);
		itemWidget->OnItemDiscarded().AddUObject(this, &UNewInventoryWidget::_onItemBeingDropped);

		_inventoryItemsBox->AddChildToWrapBox(itemWidget);
	}
}

void UNewInventoryWidget::_onItemBeingUsed(TWeakInterfacePtr<IItem> item) {
	OnItemFromInventoryBeingUsed().Broadcast(item, _inventory);
}

void UNewInventoryWidget::_onItemBeingDropped(TWeakInterfacePtr<IItem> item) {
	// In the future, the quantity to drop will be set using an additional widget. For the moment, we'll just all of
	// the item.
	OnItemFromInventoryBeingDropped().Broadcast(item, item->GetQuantity(), _inventory);
}
