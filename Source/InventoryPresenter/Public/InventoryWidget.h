// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "InventoryItemWidget.h"

#include "InventoryWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemFromInventoryUsed, TWeakInterfacePtr<IItem>, TWeakInterfacePtr<IInventory>);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemFromInventoryDiscarded, TWeakInterfacePtr<IItem>, TWeakInterfacePtr<IInventory>, const FPointerEvent&);

UCLASS()
class INVENTORYPRESENTER_API UInventoryWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void SetInventory(TWeakInterfacePtr<IInventory> inventory);

    UFUNCTION(BlueprintPure)
    TScriptInterface<IInventory> GetInventory() const { return _inventory.GetObject(); }

    FOnItemFromInventoryUsed& OnItemFromInventoryUsed() { return _onItemFromInventoryUsed; }
    FOnItemFromInventoryDiscarded& OnItemFromInventoryDiscarded() { return _onItemFromInventoryDiscarded; }

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
    void _onItemBeingDiscarded(TWeakInterfacePtr<IItem> item, const FPointerEvent& mouseEvent);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWrapBox> _inventoryItemsBox;

    TWeakInterfacePtr<IInventory> _inventory = nullptr;

    UPROPERTY()
    TSubclassOf<UObject> _currentClassFilter = nullptr;

    FOnItemFromInventoryDiscarded _onItemFromInventoryDiscarded;
    FOnItemFromInventoryUsed _onItemFromInventoryUsed;
};
