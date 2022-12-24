// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryComponent.h"

void UInventoryWidget::InitializeFromInventory(UInventoryComponent* inventoryC) {
	_itemsToWidgets.Empty();
	_inventoryItemsBox->ClearChildren();
	check(_itemWidgetClass != nullptr);
	check(IsValid(inventoryC));
	_inventory = inventoryC;

	for (const auto& item : _inventory->GetItems()) {
		auto itemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), _itemWidgetClass);
		itemWidget->SetItem(item);
		itemWidget->OnItemDoubleClick().AddUObject(this, &UInventoryWidget::_broadcastItemBeingUsedOnInventory);

		_itemsToWidgets.Emplace(item, itemWidget);

		_inventoryItemsBox->AddChildToWrapBox(itemWidget);
	}
}

void UInventoryWidget::_broadcastItemBeingUsedOnInventory(const TWeakObjectPtr<UInventoryItem> item) const {
	_onItemFromInventoryBeingUsed.Broadcast(item, _inventory);
}
