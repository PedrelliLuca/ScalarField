// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"


void UInventoryWidget::InitializeFromInventory(UInventoryComponent* inventoryC) {
	_itemsToWidgets.Empty();
	_inventoryItemsBox->ClearChildren();

	for (const auto& item : inventoryC->GetItems()) {
		auto itemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), UInventoryWidget::StaticClass());
		itemWidget->SetItem(item);

		_itemsToWidgets.Emplace(item, itemWidget);

		_inventoryItemsBox->AddChildToWrapBox(itemWidget);
	}
}
