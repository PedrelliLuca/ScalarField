// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryComponent.h"

void UInventoryWidget::InitializeFromInventory(UInventoryComponent* inventoryC) {
	check(IsValid(inventoryC));

	if (_inventory.IsValid()) {
		_inventory->OnInventoryUpdated().RemoveAll(this);
	}
	
	_inventory = inventoryC;
	_inventory->OnInventoryUpdated().AddUObject(this, &UInventoryWidget::_resetInventoryItems);

	_resetInventoryItems();
}

void UInventoryWidget::_resetInventoryItems() {
	check(_itemWidgetClass != nullptr);
	_itemsToWidgets.Empty();
	_inventoryItemsBox->ClearChildren();

	for (const auto& item : _inventory->GetItems()) {
		auto itemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), _itemWidgetClass);
		// itemWidget->SetItem(item);
		itemWidget->OnItemDoubleClick().AddUObject(this, &UInventoryWidget::_broadcastItemBeingUsedOnInventory);
		itemWidget->OnItemRightClick().AddUObject(this, &UInventoryWidget::_broadcastItemBeingDroppedFromInventory);

		_itemsToWidgets.Emplace(Cast<UInventoryItem>(item.GetObject()), itemWidget);

		_inventoryItemsBox->AddChildToWrapBox(itemWidget);
	}
}

void UInventoryWidget::_broadcastItemBeingUsedOnInventory(const TWeakObjectPtr<UInventoryItem> item) const {
	_onItemFromInventoryBeingUsed.Broadcast(item, _inventory);
}

void UInventoryWidget::_broadcastItemBeingDroppedFromInventory(TWeakObjectPtr<UInventoryItem> item) {
	OnItemDroppedFromInventory().Broadcast(item, item->GetQuantity(), _inventory);
}
