// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItemWidget.h"

void UInventoryItemWidget::SetItem(TWeakInterfacePtr<IItem> item) {
    check(item.IsValid());
    _item = item;
    RefreshWidget();
}

FReply UInventoryItemWidget::NativeOnMouseButtonDoubleClick(FGeometry const& inGeometry, FPointerEvent const& inMouseEvent) {
    check(_item.IsValid());
    OnItemUsage().Broadcast(_item);

    return Super::NativeOnMouseButtonDoubleClick(inGeometry, inMouseEvent);
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(FGeometry const& inGeometry, FPointerEvent const& inMouseEvent) {
    // https://michaeljcole.github.io/wiki.unrealengine.com/List_of_Key/Gamepad_Input_Names/
    if (inMouseEvent.IsMouseButtonDown(FKey{"RightMouseButton"})) {
        check(_item.IsValid());

        // OnItemDiscarded().Broadcast(_item, inMouseEvent.IsShiftDown() && _item->IsStackable(), inMouseEvent.GetScreenSpacePosition());
        OnItemDiscarded().Broadcast(_item, inMouseEvent);
    }

    return Super::NativeOnMouseButtonDown(inGeometry, inMouseEvent);
}
