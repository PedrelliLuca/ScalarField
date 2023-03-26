// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContainerWidgetInterface.h"
#include "ItemUsageCommandFactory.h"
#include "UObject/WeakInterfacePtr.h"

#include "ItemUsageController.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UItemUsageController : public UObject {
    GENERATED_BODY()

public:
    UItemUsageController();

    void SetItemUsageNotifier(TWeakInterfacePtr<IInventoryContainerWidget> itemUsageNotifier);
    void BindItemUsage();
    void UnbindItemUsage();

private:
    void _useItemOfInventory(TWeakInterfacePtr<IItem> item, int32 quantity, TWeakInterfacePtr<IInventory> inventory);

    TWeakInterfacePtr<IInventoryContainerWidget> _itemUsageNotifier = nullptr;

    FDelegateHandle _itemUsageHandle{};

    UPROPERTY()
    TObjectPtr<UItemUsageCommandFactory> _itemUsageCmdFactory = nullptr;
};
