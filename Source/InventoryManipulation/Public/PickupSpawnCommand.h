// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "InventoryInterface.h"
#include "PauseCommandInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "PickupSpawnCommand.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UPickupSpawnCommand : public UObject, public IPauseCommand {
     GENERATED_BODY()

     friend class UPickupSpawnCommandFactory;
     
public:
     void Execute() override;

private:
     TWeakInterfacePtr<IItem> _item = nullptr;
     TWeakInterfacePtr<IInventory> _inventory = nullptr;
     int32 _quantity = 0;
     TFunction<FTransform()> _getPickupLocationCallback = nullptr;
     TSubclassOf<AActor> _pickupClass = nullptr;
};