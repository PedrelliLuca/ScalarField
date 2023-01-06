// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ClosableWidgetInterface.h"
#include "InventoryInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "InventoryContainerWidgetInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UInventoryContainerWidget : public UInterface {
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemFromInventoryBeingUsed, TWeakInterfacePtr<IItem>, int32 quantityToUse, TWeakInterfacePtr<IInventory>);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemFromInventoryBeingDropped, TWeakInterfacePtr<IItem>, int32 quantityToDrop, TWeakInterfacePtr<IInventory>);

/* Interface for widgets that can be closed by using some UI element, like a button. */
class IInventoryContainerWidget : public IClosableWidget {
	GENERATED_BODY()

public:
	virtual void ShowInventory(TWeakInterfacePtr<IInventory> inventory) = 0;
	virtual void HideInventory() = 0;

	FOnItemFromInventoryBeingUsed& OnItemFromInventoryBeingUsed() { return _onItemFromInventoryBeingUsed; }
	FOnItemFromInventoryBeingDropped& OnItemFromInventoryBeingDropped() { return _onItemFromInventoryBeingDropped; }

private:
	FOnItemFromInventoryBeingUsed _onItemFromInventoryBeingUsed{};
	FOnItemFromInventoryBeingDropped _onItemFromInventoryBeingDropped{};
};
