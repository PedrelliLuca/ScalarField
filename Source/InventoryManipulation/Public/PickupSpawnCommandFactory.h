// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "ItemInterface.h"
#include "PauseCommandFactoryInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "PickupSpawnCommandFactory.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UPickupSpawnCommandFactory : public UObject, public IPauseCommandFactory {
    GENERATED_BODY()

public:
    TScriptInterface<IPauseCommand> CreateCommand() override;

    void SetCommandItem(TWeakInterfacePtr<IItem> item);
    void SetCommandInventory(TWeakInterfacePtr<IInventory> inventory);
    void SetCommandQuantity(int32 quantity);
    void SetPickupSpawnLocationCallback(TFunction<FTransform()> pickupLocationCallback);
    void SetPickupClass(TSubclassOf<AActor> pickupClass);

private:
    TWeakInterfacePtr<IItem> _item = nullptr;
    TWeakInterfacePtr<IInventory> _inventory = nullptr;
    int32 _quantity = 0;
    TFunction<FTransform()> _getPickupLocationCallback = nullptr;
    TSubclassOf<AActor> _pickupClass = nullptr;
};
