// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenterWidget.h"

void UInventoryPresenterWidget::ShowInventory(TWeakInterfacePtr<IInventory> inventory) {
	AddToViewport();
	_inventoryWidget->SetInventory(MoveTemp(inventory));
}

void UInventoryPresenterWidget::HideInventory() {
	RemoveFromParent();
}

TWeakInterfacePtr<IItemInventoryWidget> UInventoryPresenterWidget::GetInventoryWidget() {
	return _inventoryWidget;
}

void UInventoryPresenterWidget::_onClose() {
	OnCloseFromUI().Broadcast();
}
