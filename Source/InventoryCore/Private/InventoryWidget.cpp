// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryComponent.h"

void UInventoryWidget::InitializeFromInventory(UInventoryComponent* inventoryC) {
	_itemsToWidgets.Empty();
	_inventoryItemsBox->ClearChildren();
	check(_itemWidgetClass != nullptr);

	for (const auto& item : inventoryC->GetItems()) {
		auto itemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), _itemWidgetClass);
		itemWidget->SetItem(item);
		itemWidget->OnItemDoubleClick().AddUObject(inventoryC, &UInventoryComponent::UseItem);

		_itemsToWidgets.Emplace(item, itemWidget);

		_inventoryItemsBox->AddChildToWrapBox(itemWidget);
	}
}
