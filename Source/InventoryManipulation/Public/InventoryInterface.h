// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemAddResult.h"
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
	virtual TWeakInterfacePtr<IItem> FindItemByClass(TSubclassOf<UObject> itemClass) = 0;
	virtual FItemAddResult TryAddItem(TWeakInterfacePtr<IItem> item) = 0;
	virtual int32 ConsumeItem(TWeakInterfacePtr<IItem> item, int32 quantity) = 0;
	virtual bool RemoveItem(TWeakInterfacePtr<IItem> item) = 0;
	
	virtual TWeakObjectPtr<AActor> GetInventoryOwner() = 0;
	virtual TArray<TWeakInterfacePtr<IItem>> GetItems() const = 0;

	virtual double GetWeightCapacity() const = 0;
	virtual double GetCurrentWeight() const = 0;
	virtual double GetVolumeCapacity() const = 0;
	virtual double GetCurrentVolume() const = 0;
	
	FOnInventoryUpdated& OnInventoryUpdated() { return _onInventoryUpdated; }
	
private:
	FOnInventoryUpdated _onInventoryUpdated{};
};
