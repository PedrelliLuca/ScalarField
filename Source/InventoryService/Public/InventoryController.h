// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "WidgetsPresenterComponent.h"

#include "InventoryController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryWidgetClosed);

UCLASS()
class INVENTORYSERVICE_API UInventoryController : public UObject {
     GENERATED_BODY()
     
public:
    void SetWidgetsPresenter(TWeakObjectPtr<UWidgetsPresenterComponent>&& widgetsPresenterC);

    void ShowInventoryOfActor(TWeakObjectPtr<AActor>&& actor);

    FOnInventoryWidgetClosed& OnInventoryWidgetClosed() { return _onInventoryWidgetClosed; }

private:
    void _onInventoryClose();

	void _useItemOfInventory(TWeakObjectPtr<UInventoryItem> item, TWeakObjectPtr<UInventoryComponent> inventory);

    TWeakObjectPtr<UWidgetsPresenterComponent> _widgetsPresenterC = nullptr;

    FOnInventoryWidgetClosed _onInventoryWidgetClosed{};

	FDelegateHandle _itemUsageOnPauseToggleHandle{};
};