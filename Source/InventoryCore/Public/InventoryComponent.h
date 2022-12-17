// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Components/ActorComponent.h"

#include "InventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYCORE_API UInventoryComponent : public UActorComponent {
     GENERATED_BODY()
     
public:
     UInventoryComponent();

     UFUNCTION(BlueprintPure, Category = "Inventory")
     FORCEINLINE double GetWeightCapacity() const { return _weightCapacity; }

     UFUNCTION(BlueprintPure, Category = "Inventory")
     FORCEINLINE double GetVolumeCapacity() const { return _volumeCapacity; }

     UFUNCTION(BlueprintPure, Category = "Inventory")
     const TArray<TObjectPtr<UInventoryItem>>& GetItems() const { return _items; }

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
};
