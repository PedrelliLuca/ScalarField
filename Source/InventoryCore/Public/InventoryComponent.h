// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "InventoryItem.h"
#include "Components/ActorComponent.h"

#include "InventoryComponent.generated.h"

UENUM()
enum class EItemAddResult : uint8 {
    IAR_NoItemsAdded UMETA(DisplayName = "No items added"),
    IAR_SomeItemsAdded UMETA(DisplayName = "Some items added"),
    IAR_AllItemsAdded UMETA(DisplayName = "All items added")
};

// Represents the result of adding an item to the inventory
USTRUCT(BlueprintType)
struct FItemAddResult {
    GENERATED_BODY()

public:
    FItemAddResult() {}
    FItemAddResult(int32 itemQuantity) : ItemQuantity(itemQuantity), ItemQuantityActuallyGiven(0) {};
    FItemAddResult(int32 itemQuantity, int32 itemQuantityActuallyGiven) : ItemQuantity(itemQuantity), ItemQuantityActuallyGiven(itemQuantityActuallyGiven) {};

    // Functions that make building instances of this struct easier
    static FItemAddResult AddedNone(int32 itemQuantity, const FText& errorText);
    static FItemAddResult AddedSome(int32 itemQuantity, int32 itemQuantityActuallyGiven, const FText& errorText);
    static FItemAddResult AddedAll(int32 itemQuantity);

    UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
    int32 ItemQuantity = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
    int32 ItemQuantityActuallyGiven = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
    EItemAddResult Result = EItemAddResult::IAR_NoItemsAdded;

    UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
    FText ErrorText {};
};

// Represents an inventory for an owner actor
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYCORE_API UInventoryComponent : public UActorComponent, public IInventory {
    GENERATED_BODY()
     
public:
    UInventoryComponent();
    
    UFUNCTION(BlueprintPure, Category = "Inventory")
    FORCEINLINE double GetWeightCapacity() const { return _weightCapacity; }
    
    double GetCurrentWeight() const;
    
    double GetCurrentVolume() const;
    
    UFUNCTION(BlueprintPure, Category = "Inventory")
    FORCEINLINE double GetVolumeCapacity() const { return _volumeCapacity; }

    TWeakObjectPtr<AActor> GetInventoryOwner() override { return GetOwner(); }
    
    TArray<TWeakInterfacePtr<IItem>> GetItems() const override;

    /* Tries to add an existing item into the inventory. */
    FItemAddResult TryAddItem(TObjectPtr<UInventoryItem> item);

    /* Tries to add a certain quantity of items of a given class to the inventory. */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    FItemAddResult TryAddItemFromClass(TSubclassOf<UInventoryItem> itemClass, const int32 quantity);

    /* Takes some quantity of the item away from the inventory. Removes the item if the quantity reaches 0. The quantity of the item
    that was actually consumed (i.e. the one that takes into account how much of the item we have in the inventory) is returned.*/
    int32 ConsumeItem(TObjectPtr<UInventoryItem> item, int32 quantity);

    int32 ConsumeAllOfItem(TObjectPtr<UInventoryItem> item);

    bool RemoveItem(TObjectPtr<UInventoryItem> item);

    // We have the given amount of the input item
    bool HasItemOfClass(TSubclassOf<UInventoryItem> itemClass, int32 quantity = 1) const;

    // Returns the first item with the same class as the given item
    TWeakInterfacePtr<IItem> FindItemByClass(TSubclassOf<UObject> itemClass) override;

    // Returns all inventory items that are children  of itemClass.
    TArray<TWeakObjectPtr<UInventoryItem>> FindItemsByClass(TSubclassOf<UInventoryItem> itemClass);

protected:
    // The maximum weight this inventory can hold. This can potentially be varied using backpacks, spells, ...
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    double _weightCapacity;

    // The maximum volume this inventory can hold. This can potentially be varied using backpacks, spells, ...
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (ClampMin = 0.0))
    double _volumeCapacity;
     
    // Where we store the items in this inventory
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TArray<TObjectPtr<UInventoryItem>> _items;

private:
    // DO NOT CALL Add() or Emplace() on the _items array directly! Use this instead.
    // Adds the item to the inventory. If the input item is not owned by this inventory, makes a copy of it with this as outer
    // before adding. The item with this as outer is returned.
    TObjectPtr<UInventoryItem> _addItem(TObjectPtr<UInventoryItem> item);
};
