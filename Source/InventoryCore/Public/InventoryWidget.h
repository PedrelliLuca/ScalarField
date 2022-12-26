// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "InventoryItem.h"
#include "InventoryItemWidget.h"
#include "ItemDropperInterface.h"
#include "Delegates/DelegateCombinations.h"

#include "InventoryWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemFromInventoryBeingUsed, TWeakObjectPtr<UInventoryItem>, TWeakObjectPtr<UInventoryComponent>);

UCLASS()
class INVENTORYCORE_API UInventoryWidget : public UUserWidget, public IItemDropper {
     GENERATED_BODY()
  
public:
    UFUNCTION(BlueprintCallable)
    void InitializeFromInventory(UInventoryComponent* inventoryC);

    FOnItemFromInventoryBeingUsed& OnItemFromInventoryBeingUsed() { return _onItemFromInventoryBeingUsed; }

protected:
    UPROPERTY(EditDefaultsOnly, NoClear, Category = "Inventory Widget")
    TSubclassOf<UInventoryItemWidget> _itemWidgetClass = nullptr;

private:
    UFUNCTION()
    void _resetInventoryItems();
    
    void _broadcastItemBeingUsedOnInventory(TWeakObjectPtr<UInventoryItem> item) const;

    void _broadcastItemBeingDroppedFromInventory(TWeakObjectPtr<UInventoryItem> item);
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWrapBox> _inventoryItemsBox;

    UPROPERTY()
    TMap<TObjectPtr<UInventoryItem>, TObjectPtr<UInventoryItemWidget>> _itemsToWidgets{};

    TWeakObjectPtr<UInventoryComponent> _inventory = __nullptr;;

    FOnItemFromInventoryBeingUsed _onItemFromInventoryBeingUsed{};
};
