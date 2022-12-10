// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Engine/StaticMesh.h"

#include "InventoryItem.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8 {
     IR_Common UMETA(DisplayName = "Common"),
     IR_Uncommon UMETA(DisplayName = "Uncommon"),
     IR_Rare UMETA(DisplayName = "Rare"),
     IR_VeryRare UMETA(DisplayName = "Very Rare"),
     IR_Legendary UMETA(DisplayName = "Legendary"),
};

// EditInlineNew? DefaultToInstanced?
UCLASS(Blueprintable)
class INVENTORYSYSTEM_API UInventoryItem : public UObject {
     GENERATED_BODY()
     
public:
     UInventoryItem();

     UFUNCTION(BlueprintPure, Category = "Item")
     FORCEINLINE double GetStackWeight() const { return _quantity * _weight; }

     UFUNCTION(BlueprintPure, Category = "Item")
     virtual bool ShouldShowInInventory() const;

protected:
     // The mesh to display when the item is in the world 
     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     TObjectPtr<UStaticMesh> _worldMesh;

     // The thumbnail to display when the item is in an inventory
     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     TObjectPtr<UTexture2D> _inventoryThumbnail;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     FText _name;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
     FText _description;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     FText _action;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     EItemRarity _rarity;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 0.0))
     double _weight;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     bool _bIsStackable;

     // The maximum number of items of this kind that can be part of a single stack, i.e. of a single UInventoryItem instance
     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 2, EditCondition = _bIsStackable))
     int32 _maxQuantity;

     // The number of items of this kind that this instance is currently storing
     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (UIMin = 1, EditCondition = _bIsStackable))
     int32 _quantity;
};