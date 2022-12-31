// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "InventoryInterface.generated.h"

// We broadcast whenever the inventory updates.
DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

UINTERFACE(MinimalAPI, NotBlueprintable)
class UInventory : public UInterface {
	GENERATED_BODY()
};

/* Interface representing an inventory. */
class IInventory {
	GENERATED_BODY()

public:
	virtual TWeakObjectPtr<AActor> GetInventoryOwner() = 0;
	virtual TWeakInterfacePtr<IItem> FindItemByClass(TSubclassOf<UObject> itemClass) = 0;
	virtual TArray<TWeakInterfacePtr<IItem>> GetItems() const = 0;
	
	FOnInventoryUpdated& OnInventoryUpdated() { return _onInventoryUpdated; }
	
private:
	FOnInventoryUpdated _onInventoryUpdated{};
};
