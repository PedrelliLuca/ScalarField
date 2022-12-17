// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

void UInventoryItemWidget::SetItem(const TObjectPtr<UInventoryItem>& inventoryItem) {
	check(IsValid(inventoryItem));
	_inventoryItem = inventoryItem;
	_refresh();
}
