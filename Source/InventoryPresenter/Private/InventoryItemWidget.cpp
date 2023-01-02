// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

void UInventoryItemWidget::SetItem(TWeakInterfacePtr<IItem> item) {
	check(item.IsValid());
	_item = item;
	RefreshWidget();
}

FReply UInventoryItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) {
	check(_item.IsValid());	
	OnItemUsage().Broadcast(_item);
	
	return Super::NativeOnMouseButtonDoubleClick(inGeometry, inMouseEvent);
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) {
	// https://michaeljcole.github.io/wiki.unrealengine.com/List_of_Key/Gamepad_Input_Names/
	if (inMouseEvent.IsMouseButtonDown(FKey{"RightMouseButton"})) {
		check(_item.IsValid());	
		OnItemDiscarded().Broadcast(_item);
	}
	
	return Super::NativeOnMouseButtonDown(inGeometry, inMouseEvent);
}
