// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "Engine/StaticMesh.h"

#include "InventoryItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemModified);

UENUM(BlueprintType)
enum class EItemRarity : uint8 {
     IR_Common UMETA(DisplayName = "Common"),
     IR_Uncommon UMETA(DisplayName = "Uncommon"),
     IR_Rare UMETA(DisplayName = "Rare"),
     IR_VeryRare UMETA(DisplayName = "Very Rare"),
     IR_Legendary UMETA(DisplayName = "Legendary"),
};

// EditInlineNew? DefaultToInstanced?
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class INVENTORYCORE_API UInventoryItem : public UObject, public IItem {
     GENERATED_BODY()
     
public:
     UInventoryItem();

#if WITH_EDITOR
     void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

     UFUNCTION(BlueprintPure, Category = "Item")
     FORCEINLINE double GetStackWeight() const { return _quantity * _weight; }

     FORCEINLINE double GetWeight() const { return _weight; }

     UFUNCTION(BlueprintPure, Category = "Item")
     FORCEINLINE double GetStackVolume() const { return _quantity * _volume; }

     FORCEINLINE double GetVolume() const { return _volume; }

     bool IsStackable() const override { return _bIsStackable; }

     bool ShouldShowInInventory() const override { return true; }

     int32 GetMaxQuantity() const override { return _maxQuantity; }

     const FText& GetNameText() const override { return _nameText; }

     TObjectPtr<UTexture2D> GetThumbnail() override { return _inventoryThumbnail; } 

     FORCEINLINE const FText& GetActionText() const { return _actionText; }

     UFUNCTION(BlueprintCallable, Category = "Item")
     void SetQuantity(int32 newQuantity) override;

     int32 GetQuantity() const override { return _quantity; }

     FORCEINLINE TObjectPtr<UStaticMesh> GetMesh() { return _worldMesh; }

     void Use(TWeakObjectPtr<AActor> actor) override {}

     // Called to execute some logic when the item is added to the given inventory
     virtual void OnItemAddedToInventory(TWeakObjectPtr<class UInventoryComponent> inventory) {}

     FOnItemModified& OnItemModified() { return _onItemModified; }

protected:
     // The mesh to display when the item is in the world 
     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     TObjectPtr<UStaticMesh> _worldMesh;

     // The thumbnail to display when the item is in an inventory
     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     TObjectPtr<UTexture2D> _inventoryThumbnail;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     FText _nameText;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
     FText _descriptionText;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     FText _actionText;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     EItemRarity _rarity;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 0.0))
     double _weight;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 0.0))
     double _volume;

     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
     bool _bIsStackable;

     // The maximum number of items of this kind that can be part of a single stack, i.e. of a single UInventoryItem instance
     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 2, EditCondition = _bIsStackable))
     int32 _maxQuantity;

     // The number of items of this kind that this instance is currently storing
     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (UIMin = 1, EditCondition = _bIsStackable))
     int32 _quantity;

     UPROPERTY(BlueprintAssignable)
     FOnItemModified _onItemModified;
};