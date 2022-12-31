// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInventoryWidgetInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "NewPickupSpawnController.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UNewPickupSpawnController : public UObject {
     GENERATED_BODY()
     
public:
     UNewPickupSpawnController();
     
     void SetItemDropNotifier(TWeakInterfacePtr<IItemInventoryWidget> itemDropNotifier);
     void SetPickupSpawnCallback(TFunction<FTransform()>&& pickupSpawnCallback);
     void BindPickupSpawn();
     void UnbindPickupSpawn();

private:
     void _spawnPickup(TWeakInterfacePtr<IItem> item, int32 quantity, TWeakInterfacePtr<IInventory> inventory);

     TSubclassOf<AActor> _pickupClass = nullptr;
     
     TWeakInterfacePtr<IItemInventoryWidget> _itemDropNotifier = nullptr;

     FDelegateHandle _itemDropHandle{};

     TFunction<FTransform()> _getPickupSpawnLocation;
};
