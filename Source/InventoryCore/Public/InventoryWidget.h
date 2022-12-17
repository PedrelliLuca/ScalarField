// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"

#include "InventoryWidget.generated.h"

UCLASS()
class INVENTORYCORE_API UInventoryWidget : public UUserWidget {
     GENERATED_BODY()
  
public:
    UFUNCTION(BlueprintCallable)
    void InitializeFromInventory(UInventoryComponent* inventoryC);
};
