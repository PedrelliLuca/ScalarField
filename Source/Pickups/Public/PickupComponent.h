// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableComponent.h"
#include "InteractorInterface.h"
#include "InventoryItem.h"

#include "PickupComponent.generated.h"

/**
 * \brief Component that manages how the widgets behave with respect to each other
 */
UCLASS()
class PICKUPS_API UPickupComponent : public UActorComponent {
     GENERATED_BODY()
     
public:
    UPickupComponent();

    // Given an item, makes a pickup out of it.
    void InitializePickup(TObjectPtr<const UInventoryItem> itemTemplate);

protected:
    void BeginPlay() override;

private:
    UFUNCTION()
    void _onItemModified();

    UFUNCTION()
    void _onTakePickup(TScriptInterface<IInteractor> interactor);

    UPROPERTY()
    TObjectPtr<UInventoryItem> _item = nullptr;

    TWeakObjectPtr<UStaticMeshComponent> _meshC = nullptr;

    TWeakObjectPtr<UInteractableComponent> _interactableC = nullptr;
};
