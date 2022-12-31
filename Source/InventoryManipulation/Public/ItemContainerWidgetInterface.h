// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/WeakInterfacePtr.h"
#include "InventoryInterface.h"

#include "ItemContainerWidgetInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStoredItemBeingUsed, TWeakInterfacePtr<IItem>, TWeakInterfacePtr<IInventory>);

UINTERFACE(MinimalAPI, NotBlueprintable)
class UItemContainerWidget : public UInterface {
	GENERATED_BODY()
};

/* Interface for widgets that can be closed by using some UI element, like a button. */
class IItemContainerWidget {
	GENERATED_BODY()

public:
	virtual void SetInventory(TWeakInterfacePtr<IInventory> inventory) = 0;

	FOnStoredItemBeingUsed& OnStoredItemBeingUsed() { return _onStoredItemBeingUsed; }
	
private:
	FOnStoredItemBeingUsed _onStoredItemBeingUsed{};
};
