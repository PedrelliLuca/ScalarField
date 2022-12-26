// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InventoryItem.h"

#include "ItemDropperInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemDroppedFromInventory, TWeakObjectPtr<UInventoryItem> item, int32 quantityToDrop, TWeakObjectPtr<UInventoryComponent> inventory);

UINTERFACE(MinimalAPI, NotBlueprintable)
class UItemDropper : public UInterface {
	GENERATED_BODY()
};

class IItemDropper {
	GENERATED_BODY()

public:
	FOnItemDroppedFromInventory& OnItemDroppedFromInventory() { return _onItemDroppedFromInventory; }
	
private:
	FOnItemDroppedFromInventory _onItemDroppedFromInventory{};
};
