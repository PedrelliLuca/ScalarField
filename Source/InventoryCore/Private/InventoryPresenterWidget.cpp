// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenterWidget.h"


void UInventoryPresenterWidget::NativeConstruct() {
	Super::NativeConstruct();

	const TWeakObjectPtr<UInventoryComponent> inventoryC = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	check(inventoryC.IsValid());

	_inventoryWidget->InitializeFromInventory(inventoryC.Get());
}

void UInventoryPresenterWidget::_onClose() {
	_onButtonClose.Broadcast();
}
