// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

void UInventoryWidget::SetInventory(TWeakInterfacePtr<IInventory> inventory) {
	if (_inventory.IsValid()) {
		_inventory->OnInventoryUpdated().RemoveAll(this);
	}

	check(inventory.IsValid());
	_inventory = MoveTemp(inventory);
	_inventory->OnInventoryUpdated().AddUObject(this, &UInventoryWidget::_resetInventoryItems);

	_resetInventoryItems();
}

void UInventoryWidget::_resetInventoryItems() {
	check(IsValid(_itemWidgetClass));
	_inventoryItemsBox->ClearChildren();

	for (const auto& item : _inventory->GetItems()) {
		const auto itemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), _itemWidgetClass);
		itemWidget->SetItem(item);
		itemWidget->OnItemUsage().AddUObject(this, &UInventoryWidget::_onItemBeingUsed);
		itemWidget->OnItemDiscarded().AddUObject(this, &UInventoryWidget::_onItemBeingDropped);

		_inventoryItemsBox->AddChildToWrapBox(itemWidget);
	}
}

void UInventoryWidget::_onItemBeingUsed(TWeakInterfacePtr<IItem> item) {
	OnItemFromInventoryBeingUsed().Broadcast(item, _inventory);
}

void UInventoryWidget::_onItemBeingDropped(TWeakInterfacePtr<IItem> item) {
	// In the future, the quantity to drop will be set using an additional widget. For the moment, we'll just all of
	// the item.
	OnItemFromInventoryBeingDropped().Broadcast(item, item->GetQuantity(), _inventory);
}
