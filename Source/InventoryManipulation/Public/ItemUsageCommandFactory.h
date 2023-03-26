// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "ItemInterface.h"
#include "PauseCommandFactoryInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "ItemUsageCommandFactory.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UItemUsageCommandFactory : public UObject, public IPauseCommandFactory {
    GENERATED_BODY()

public:
    TScriptInterface<IPauseCommand> CreateCommand() override;

    void SetCommandItem(TWeakInterfacePtr<IItem> item);
    void SetCommandInventory(TWeakInterfacePtr<IInventory> inventory);
    void SetCommandQuantity(int32 quantity);

private:
    TWeakInterfacePtr<IItem> _item = nullptr;
    TWeakInterfacePtr<IInventory> _inventory = nullptr;
    int32 _quantity = 0;
};
