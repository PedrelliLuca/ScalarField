// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemUsageCommandFactory.h"

#include "ItemUsageCommand.h"

TScriptInterface<IPauseCommand> UItemUsageCommandFactory::CreateCommand() {
    const auto command = NewObject<UItemUsageCommand>(this, UItemUsageCommand::StaticClass());
    command->_item = MoveTemp(_item);
    command->_inventory = MoveTemp(_inventory);
    command->_quantity = _quantity;
    _quantity = 0;

    return command;
}

void UItemUsageCommandFactory::SetCommandItem(TWeakInterfacePtr<IItem> item) {
    check(item.IsValid());
    _item = MoveTemp(item);
}

void UItemUsageCommandFactory::SetCommandInventory(TWeakInterfacePtr<IInventory> inventory) {
    check(inventory.IsValid());
    _inventory = MoveTemp(inventory);
}

void UItemUsageCommandFactory::SetCommandQuantity(int32 quantity) {
    check(quantity > 0);
    _quantity = quantity;
}
