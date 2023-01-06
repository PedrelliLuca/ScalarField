// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/WeakInterfacePtr.h"
#include "InventoryInterface.h"

#include "ItemInventoryWidgetInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStoredItemBeingUsed, TWeakInterfacePtr<IItem>, int32 quantityToUse, TWeakInterfacePtr<IInventory>);

UINTERFACE(MinimalAPI, NotBlueprintable)
class UItemInventoryWidget : public UInterface {
	GENERATED_BODY()
};

/* Interface for widgets that can be closed by using some UI element, like a button. */
class IItemInventoryWidget {
	GENERATED_BODY()

public:
	virtual void SetInventory(TWeakInterfacePtr<IInventory> inventory) = 0;

	FOnStoredItemBeingUsed& OnItemFromInventoryBeingUsed() { return _onItemFromInventoryBeingUsed; }
	
private:
	FOnStoredItemBeingUsed _onItemFromInventoryBeingUsed{};
};
