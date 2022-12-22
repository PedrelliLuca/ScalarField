// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "InventoryItem.h"
#include "InventoryItemWidget.h"

#include "InventoryWidget.generated.h"

UCLASS()
class INVENTORYCORE_API UInventoryWidget : public UUserWidget {
     GENERATED_BODY()
  
public:
    UFUNCTION(BlueprintCallable)
    void InitializeFromInventory(UInventoryComponent* inventoryC);

protected:
    UPROPERTY(EditDefaultsOnly, NoClear, Category = "Inventory Widget")
    TSubclassOf<UInventoryItemWidget> _itemWidgetClass = nullptr;

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWrapBox> _inventoryItemsBox;

    UPROPERTY()
    TMap<TObjectPtr<UInventoryItem>, TObjectPtr<UInventoryItemWidget>> _itemsToWidgets{};
};
