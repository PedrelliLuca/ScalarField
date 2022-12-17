// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "InventoryItem.h"

#include "InventoryItemWidget.generated.h"

UCLASS()
class INVENTORYCORE_API UInventoryItemWidget : public UUserWidget {
     GENERATED_BODY()
  
public:
    void SetItem(const TObjectPtr<UInventoryItem>& inventoryItem);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void _refresh();

    UPROPERTY(BlueprintReadOnly, Category = "Inventory Item Widget", meta = (ExposeOnSpawn = true))
    TObjectPtr<UInventoryItem> _inventoryItem = nullptr;
};
