// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemUsageController.h"

#include "TacticalPauseWorldSubsystem.h"

UItemUsageController::UItemUsageController() {
    _itemUsageCmdFactory = CreateDefaultSubobject<UItemUsageCommandFactory>(TEXT("Item Usage Command Factory"));
}

void UItemUsageController::SetItemUsageNotifier(TWeakInterfacePtr<IInventoryContainerWidget> itemUsageNotifier) {
    if (_itemUsageNotifier.IsValid()) {
        _itemUsageNotifier->OnItemFromInventoryBeingUsed().Remove(_itemUsageHandle);
        _itemUsageHandle.Reset();
    }

    check(itemUsageNotifier.IsValid());
    _itemUsageNotifier = MoveTemp(itemUsageNotifier);
}

void UItemUsageController::BindItemUsage() {
    check(!_itemUsageHandle.IsValid() && _itemUsageNotifier.IsValid());
    _itemUsageHandle = _itemUsageNotifier->OnItemFromInventoryBeingUsed().AddUObject(this, &UItemUsageController::_useItemOfInventory);
}

void UItemUsageController::UnbindItemUsage() {
    check(_itemUsageNotifier.IsValid());
    _itemUsageNotifier->OnItemFromInventoryBeingUsed().Remove(_itemUsageHandle);
    _itemUsageHandle.Reset();
}

void UItemUsageController::_useItemOfInventory(TWeakInterfacePtr<IItem> item, int32 const quantity, TWeakInterfacePtr<IInventory> inventory) {
    check(item.IsValid() && inventory.IsValid());
    auto const pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();

    if (!pauseSubsys->IsTacticalPauseOn()) {
        // We are using the item from the inventory => we expect that the inventory contains the item
        check(inventory->FindItemByClass(item.GetObject()->GetClass()).IsValid());

        item->Use(inventory->GetInventoryOwner());

        if (item->DoesUseConsume()) {
            inventory->ConsumeItem(item, quantity);
        }

        return;
    }

    check(IsValid(_itemUsageCmdFactory));
    _itemUsageCmdFactory->SetCommandItem(MoveTemp(item));
    _itemUsageCmdFactory->SetCommandInventory(MoveTemp(inventory));
    _itemUsageCmdFactory->SetCommandQuantity(quantity);
    pauseSubsys->SetPauseOffCommand(_itemUsageCmdFactory->CreateCommand());
}
