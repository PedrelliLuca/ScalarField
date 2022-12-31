// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerWidgetInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "ItemUsageController.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UItemUsageController : public UObject {
     GENERATED_BODY()
     
public:
     void SetItemContainerWidget(TWeakInterfacePtr<IItemContainerWidget> itemContainer);
     void BindItemUsage();
     void UnbindItemUsage();

private:
     void _useItemOfInventory(TWeakInterfacePtr<IItem> item, TWeakInterfacePtr<IInventory> inventory);
     
     TWeakInterfacePtr<IItemContainerWidget> _itemContainer = nullptr;

     FDelegateHandle _itemUsageHandle{};
     FDelegateHandle _itemUsageOnPauseToggleHandle{};
};