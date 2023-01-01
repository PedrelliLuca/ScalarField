// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInventoryWidgetInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "ItemUsageController.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UItemUsageController : public UObject {
     GENERATED_BODY()
     
public:
     void SetItemUsageNotifier(TWeakInterfacePtr<IItemInventoryWidget> itemUsageNotifier);
     void BindItemUsage();
     void UnbindItemUsage();

private:
     void _useItemOfInventory(TWeakInterfacePtr<IItem> item, int32 quantity, TWeakInterfacePtr<IInventory> inventory);
     
     TWeakInterfacePtr<IItemInventoryWidget> _itemUsageNotifier = nullptr;

     FDelegateHandle _itemUsageHandle{};
     FDelegateHandle _itemUsageOnPauseToggleHandle{};
};