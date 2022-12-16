// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryPresenterWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnButtonClose);

UCLASS()
class INVENTORYCORE_API UInventoryPresenterWidget : public UUserWidget {
     GENERATED_BODY()
  
public:
    FOnButtonClose& OnButtonClose() { return _onButtonClose; }

protected:
    UFUNCTION(BlueprintCallable)
    void _onClose();

private:
    FOnButtonClose _onButtonClose{};
};
