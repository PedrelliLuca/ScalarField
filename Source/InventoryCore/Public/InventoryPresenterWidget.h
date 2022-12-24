// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.h"

#include "InventoryPresenterWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnButtonClose);

UCLASS()
class INVENTORYCORE_API UInventoryPresenterWidget : public UUserWidget {
     GENERATED_BODY()
  
public:
    FOnButtonClose& OnButtonClose() { return _onButtonClose; }

    TWeakObjectPtr<UInventoryWidget> GetInventoryWidget() { return _inventoryWidget; }

protected:
    void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void _onClose();

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInventoryWidget> _inventoryWidget;

    FOnButtonClose _onButtonClose{};
};
