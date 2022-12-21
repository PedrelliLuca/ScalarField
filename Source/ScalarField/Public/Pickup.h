// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "InteractableComponent.h"
#include "InventoryItem.h"
#include "PickupComponent.h"

#include "Pickup.generated.h"

UCLASS()
class SCALARFIELD_API APickup : public AActor {
     GENERATED_BODY()
     
public:
    APickup();

#if WITH_EDITOR
     void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

protected:
    void BeginPlay() override;

private:
    // Aligns mesh rotation with ground rotation.
    void _alignWithGround();

    // Used as template to create the pickup when spawned in.
    UPROPERTY(EditAnywhere, Instanced)
    TObjectPtr<UInventoryItem> _itemTemplate = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> _meshC;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UInteractableComponent> _interactableC;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UPickupComponent> _pickupC;
};