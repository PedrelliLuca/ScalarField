// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryPresenterWidget.generated.h"

UCLASS()
class INVENTORYCORE_API UInventoryPresenterWidget : public UUserWidget {
     GENERATED_BODY()
     
public:
    UFUNCTION(BlueprintCallable)
    void OnClose();
};
