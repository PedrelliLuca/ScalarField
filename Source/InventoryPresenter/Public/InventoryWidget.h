// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Components/Overlay.h"
#include "ItemInventoryWidgetInterface.h"
#include "InventoryItemWidget.h"
#include "QuantitySetterWidget.h"

#include "InventoryWidget.generated.h"

UCLASS()
class INVENTORYPRESENTER_API UInventoryWidget : public UUserWidget, public IItemInventoryWidget {
     GENERATED_BODY()
  
public:
     void SetInventory(TWeakInterfacePtr<IInventory> inventory) override;

     UFUNCTION(BlueprintPure)
     TScriptInterface<IInventory> GetInventory() const { return _inventory.GetObject(); }

protected:
     UPROPERTY(EditDefaultsOnly, NoClear, Category = "Inventory Widget")
     TSubclassOf<UInventoryItemWidget> _itemWidgetClass = nullptr;

     UFUNCTION(BlueprintCallable)
     void _filterItemsByClass(TSubclassOf<UObject> itemClass = nullptr);

     UFUNCTION(BlueprintImplementableEvent)
     void _refreshInventoryInformation();
     
private:
     void _refreshInventoryItems();

     void _onItemBeingUsed(TWeakInterfacePtr<IItem> item);
     void _onItemBeingDropped(TWeakInterfacePtr<IItem> item, const FPointerEvent& mouseEvent);

     UFUNCTION()
     void _sanitizeItemQuantity(const FText& quantityText, ETextCommit::Type commitType);
     
     UPROPERTY(meta = (BindWidget))
     TObjectPtr<UWrapBox> _inventoryItemsBox;

     TWeakInterfacePtr<IInventory> _inventory = nullptr;

     UPROPERTY()
     TSubclassOf<UObject> _currentClassFilter = nullptr;

     UPROPERTY(meta = (BindWidget))
     TObjectPtr<UOverlay> _inventoryOverlay;

     UPROPERTY(EditDefaultsOnly)
     TSubclassOf<UQuantitySetterWidget> _quantitySetterClass = nullptr;
     
     UPROPERTY()
     TObjectPtr<UQuantitySetterWidget> _quantitySetter = nullptr;

     TWeakInterfacePtr<IItem> _itemPendingDrop = nullptr;
};
