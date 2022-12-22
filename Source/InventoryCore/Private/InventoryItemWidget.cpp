// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

void UInventoryItemWidget::SetItem(const TObjectPtr<UInventoryItem>& inventoryItem) {
	check(IsValid(inventoryItem));
	_inventoryItem = inventoryItem;
	RefreshWidget();
}

FReply UInventoryItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) {
	check(IsValid(_inventoryItem));
	_onDoubleClick.Broadcast(_inventoryItem);
	
	return Super::NativeOnMouseButtonDoubleClick(inGeometry, inMouseEvent);
}
