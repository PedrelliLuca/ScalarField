// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "ItemDropperInterface.h"


#include "PickupSpawnController.generated.h"

UCLASS()
class PICKUPS_API UPickupSpawnController : public UObject {
     GENERATED_BODY()
     
public:
     UPickupSpawnController();
     
     void RegisterItemDropper(TScriptInterface<IItemDropper> itemDropper);

private:
     void _spawnPickup(TWeakObjectPtr<UInventoryItem> item, int32 quantityToDrop, TWeakObjectPtr<UInventoryComponent> inventory);

     UPROPERTY()
     TSubclassOf<AActor> _pickupClass = nullptr;
};