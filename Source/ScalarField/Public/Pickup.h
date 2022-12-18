// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "InteractableComponent.h"
#include "PickupComponent.h"

#include "Pickup.generated.h"

UCLASS()
class SCALARFIELD_API APickup : public AActor {
     GENERATED_BODY()
     
public:
    APickup();

protected:
    void BeginPlay() override;

private:
    // Aligns mesh rotation with ground rotation.
    void _alignWithGround();

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> _meshC;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UInteractableComponent> _interactableC;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UPickupComponent> _pickupC;
};