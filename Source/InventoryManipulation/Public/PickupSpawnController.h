// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HealthComponent.h"
#include "InventoryContainerWidgetInterface.h"
#include "PickupSpawnCommandFactory.h"
#include "UObject/WeakInterfacePtr.h"

#include "PickupSpawnController.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UPickupSpawnController : public UObject {
    GENERATED_BODY()

public:
    UPickupSpawnController();

    void SetItemDropNotifier(TWeakInterfacePtr<IInventoryContainerWidget> itemDropNotifier);
    void SetPickupSpawnCallback(TFunction<FTransform()>&& pickupSpawnCallback);
    void BindPickupSpawn();
    void UnbindPickupSpawn();

    /** \brief Sets up a callback so that the given actor will drop the content of its inventory at death. The actor must have a UHealthComponent and a
     * UInventoryComponent for this to work. */
    void BindPickupsDropAtDeath(const TObjectPtr<AActor> actor);

private:
    void _spawnPickup(TWeakInterfacePtr<IItem> item, int32 quantity, TWeakInterfacePtr<IInventory> inventory);

    void _dropPickupsOnDeath(TObjectPtr<AActor> deadActor);

    TSubclassOf<AActor> _pickupClass = nullptr;

    TWeakInterfacePtr<IInventoryContainerWidget> _itemDropNotifier = nullptr;

    FDelegateHandle _itemDropHandle{};

    TFunction<FTransform()> _getPickupSpawnLocation;

    UPROPERTY()
    TObjectPtr<UPickupSpawnCommandFactory> _pickupSpawnCmdFactory = nullptr;

    struct FDeathDropParams {
        TWeakObjectPtr<UHealthComponent> DeadHealthC;
        TWeakInterfacePtr<IInventory> InventoryToDrop;
        FDelegateHandle OnDeathHandle;
    };

    TMap<TWeakObjectPtr<AActor>, FDeathDropParams> _actorToDeathDropParams;
};
