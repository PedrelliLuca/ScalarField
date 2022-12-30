// Fill out your copyright notice in the Description page of Project Settings.


#include "NewInventoryPresenterWidget.h"

void UNewInventoryPresenterWidget::ShowInventory(TWeakInterfacePtr<IInventory> inventory) {
	AddToViewport();
	_inventoryWidget->SetInventory(MoveTemp(inventory));
}

void UNewInventoryPresenterWidget::HideInventory() {
	RemoveFromParent();
}

void UNewInventoryPresenterWidget::_onClose() {
	OnCloseFromUI().Broadcast();
}
