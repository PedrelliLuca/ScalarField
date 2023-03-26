// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"

void UInventoryWidget::SetInventory(TWeakInterfacePtr<IInventory> inventory) {
    if (_inventory.IsValid()) {
        _inventory->OnInventoryUpdated().RemoveAll(this);
    }

    check(inventory.IsValid());
    _inventory = MoveTemp(inventory);
    _inventory->OnInventoryUpdated().AddUObject(this, &UInventoryWidget::_refreshInventoryItems);

    _currentClassFilter = nullptr;
    _refreshInventoryItems();
}

void UInventoryWidget::_filterItemsByClass(TSubclassOf<UObject> itemClass /*= nullptr*/) {
    check(IsValid(_itemWidgetClass));
    _inventoryItemsBox->ClearChildren();

#if DO_CHECK
    if (IsValid(itemClass)) {
        check(itemClass->ImplementsInterface(UItem::StaticClass()));
    }
#endif

    for (const auto& item : _inventory->GetItems()) {
        if (IsValid(itemClass) && !item.GetObject()->IsA(itemClass)) {
            continue;
        }

        const auto itemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), _itemWidgetClass);
        itemWidget->SetItem(item);
        itemWidget->OnItemUsage().AddUObject(this, &UInventoryWidget::_onItemBeingUsed);
        itemWidget->OnItemDiscarded().AddUObject(this, &UInventoryWidget::_onItemBeingDiscarded);

        _inventoryItemsBox->AddChildToWrapBox(itemWidget);
    }

    _currentClassFilter = MoveTemp(itemClass);

    _refreshInventoryInformation();
}

void UInventoryWidget::_refreshInventoryItems() {
    _filterItemsByClass(_currentClassFilter);
}

void UInventoryWidget::_onItemBeingUsed(TWeakInterfacePtr<IItem> item) {
    _onItemFromInventoryUsed.Broadcast(item, _inventory);
}

void UInventoryWidget::_onItemBeingDiscarded(TWeakInterfacePtr<IItem> item, const FPointerEvent& mouseEvent) {
    _onItemFromInventoryDiscarded.Broadcast(item, _inventory, mouseEvent);
}
