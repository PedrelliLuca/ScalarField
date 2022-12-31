// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "ItemInventoryWidgetInterface.h"
#include "InventoryItemWidget.h"

#include "InventoryWidget.generated.h"

UCLASS()
class INVENTORYPRESENTER_API UInventoryWidget : public UUserWidget, public IItemInventoryWidget {
     GENERATED_BODY()
  
public:
     void SetInventory(TWeakInterfacePtr<IInventory> inventory) override;


protected:
     UPROPERTY(EditDefaultsOnly, NoClear, Category = "Inventory Widget")
     TSubclassOf<UInventoryItemWidget> _itemWidgetClass = nullptr;

private:
     void _resetInventoryItems();

     void _onItemBeingUsed(TWeakInterfacePtr<IItem> item);
     void _onItemBeingDropped(TWeakInterfacePtr<IItem> item);

     UPROPERTY(meta = (BindWidget))
     TObjectPtr<UWrapBox> _inventoryItemsBox;

     TWeakInterfacePtr<IInventory> _inventory = nullptr;
};
