// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ClosableWidgetInterface.h"
#include "InventoryInterface.h"
#include "ItemContainerWidgetInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "InventoryContainerWidgetInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UInventoryContainerWidget : public UInterface {
	GENERATED_BODY()
};

/* Interface for widgets that can be closed by using some UI element, like a button. */
class IInventoryContainerWidget : public IClosableWidget {
	GENERATED_BODY()

public:
	virtual void ShowInventory(TWeakInterfacePtr<IInventory> inventory) = 0;
	virtual void HideInventory() = 0;
	virtual TWeakInterfacePtr<IItemContainerWidget> GetInventoryWidget() = 0;
};
