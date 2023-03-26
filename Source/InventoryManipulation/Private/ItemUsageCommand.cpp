// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemUsageCommand.h"

void UItemUsageCommand::Execute() {
    check(_item.IsValid() && _inventory.IsValid());

    check(_inventory->FindItemByClass(_item.GetObject()->GetClass()).IsValid());

    _item->Use(_inventory->GetInventoryOwner());

    if (_item->DoesUseConsume()) {
        _inventory->ConsumeItem(_item, _quantity);
    }
}
