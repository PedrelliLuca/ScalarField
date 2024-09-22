// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "InventoryItem.h"

#include "InventoryComponent.generated.h"

// Represents an inventory for an owner actor
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYCORE_API UInventoryComponent : public UActorComponent, public IInventory {
    GENERATED_BODY()

public:
    UInventoryComponent();

    UFUNCTION(BlueprintPure, Category = "Inventory")
    FORCEINLINE double GetWeightCapacity() const override { return _weightCapacity; }

    UFUNCTION(BlueprintPure, Category = "Inventory")
    double GetCurrentWeight() const override;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    double GetCurrentVolume() const override;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    FORCEINLINE double GetVolumeCapacity() const override { return _volumeCapacity; }

    TWeakObjectPtr<AActor> GetInventoryOwner() override { return GetOwner(); }

    TArray<TWeakInterfacePtr<IItem>> GetItems() const override;

    /* Tries to add an existing item into the inventory. */
    FItemAddResult TryAddItem(TWeakInterfacePtr<IItem> item) override;

    /* Tries to add a certain quantity of items of a given class to the inventory. */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    FItemAddResult TryAddItemFromClass(TSubclassOf<UInventoryItem> itemClass, int32 const quantity);

    /* Takes some quantity of the item away from the inventory. Removes the item if the quantity reaches 0. The quantity of the item
    that was actually consumed (i.e. the one that takes into account how much of the item we have in the inventory) is returned.*/
    int32 ConsumeItem(TWeakInterfacePtr<IItem> item, int32 quantity) override;

    int32 ConsumeAllOfItem(TObjectPtr<UInventoryItem> item);

    bool RemoveItem(TWeakInterfacePtr<IItem> item) override;

    // We have the given amount of the input item
    bool HasItemOfClass(TSubclassOf<UInventoryItem> itemClass, int32 quantity = 1) const;

    // Returns the first item with the same class as the given item
    TWeakInterfacePtr<IItem> FindItemByClass(TSubclassOf<UObject> itemClass) override;

    // Returns all inventory items that are children  of itemClass.
    TArray<TWeakObjectPtr<UInventoryItem>> FindItemsByClass(TSubclassOf<UInventoryItem> itemClass);

protected:
    void BeginPlay() override;

    // The maximum weight this inventory can hold. This can potentially be varied using backpacks, spells, ...
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    double _weightCapacity;

    // The maximum volume this inventory can hold. This can potentially be varied using backpacks, spells, ...
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (ClampMin = 0.0))
    double _volumeCapacity;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    bool _dropItemsAtDeath = true;

    // Where we store the items in this inventory
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TArray<TObjectPtr<UInventoryItem>> _items;

    // Allows for a set of initial items to be added to our instances.
    UPROPERTY(EditAnywhere, Instanced, Category = "Inventory")
    TArray<UInventoryItem*> _startingItems;

private:
    // DO NOT CALL Add() or Emplace() on the _items array directly! Use this instead.
    // Adds the item to the inventory. If the input item is not owned by this inventory, makes a copy of it with this as outer
    // before adding. The item with this as outer is returned.
    TObjectPtr<UInventoryItem> _addItem(TWeakInterfacePtr<IItem> const item);
};
