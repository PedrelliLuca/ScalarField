// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InventoryItem.h"

#include "PickupInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UPickup : public UInterface {
	GENERATED_BODY()
};

class IPickup {
	GENERATED_BODY()

public:
	virtual void InitializePickup(TSubclassOf<UInventoryItem> itemClass, int32 quantity) = 0;
};
