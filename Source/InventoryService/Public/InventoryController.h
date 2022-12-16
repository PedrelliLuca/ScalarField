// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetsPresenterComponent.h"

#include "InventoryController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryWidgetClosed);

// EditInlineNew? DefaultToInstanced?
UCLASS()
class INVENTORYSERVICE_API UInventoryController : public UObject {
     GENERATED_BODY()
     
public:
    void SetWidgetsPresenter(TWeakObjectPtr<UWidgetsPresenterComponent>&& widgetsPresenterC);

    void ShowInventoryOfActor(TWeakObjectPtr<AActor>&& actor);

    FOnInventoryWidgetClosed& OnInventoryWidgetClosed() { return _onInventoryWidgetClosed; }

private:
    void _onInventoryClose();

    TWeakObjectPtr<UWidgetsPresenterComponent> _widgetsPresenterC = nullptr;

    FOnInventoryWidgetClosed _onInventoryWidgetClosed{};
};