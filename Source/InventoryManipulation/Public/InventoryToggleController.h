// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContainerWidgetInterface.h"
#include "PawnBindableWidgetInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "InventoryToggleController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryClosedFromUI);

UCLASS()
class INVENTORYMANIPULATION_API UInventoryToggleController : public UObject {
    GENERATED_BODY()

public:
    void SetInventoryContainerWidget(TWeakInterfacePtr<IInventoryContainerWidget> inventoryPresenter);
    void SetHUDToShowOnClose(TWeakInterfacePtr<IPawnBindableWidget>&& widgetOnClose);

    void OpenInventoryOfActor(TWeakObjectPtr<AActor>&& actor);
    void CloseInventory();

    FOnInventoryClosedFromUI& OnInventoryClosedFromUI() { return _onInventoryClosedFromUI; }

private:
    void _inventoryWidgetClosedCallback();

    TWeakInterfacePtr<IInventoryContainerWidget> _inventoryContainer = nullptr;
    TWeakInterfacePtr<IPawnBindableWidget> _hudToShowOnClose = nullptr;

    FOnInventoryClosedFromUI _onInventoryClosedFromUI{};
};
