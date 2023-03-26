// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableComponent.h"
#include "InteractorInterface.h"
#include "ItemInterface.h"

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
    void InitializePickup(TSubclassOf<UObject> itemClass, int32 quantity);

protected:
    void BeginPlay() override;

private:
    UFUNCTION()
    void _onItemModified();

    UFUNCTION()
    void _onTakePickup(TScriptInterface<IInteractor> interactor);

    UPROPERTY()
    TScriptInterface<IItem> _item = nullptr;

    TWeakObjectPtr<UStaticMeshComponent> _meshC = nullptr;

    TWeakObjectPtr<UInteractableComponent> _interactableC = nullptr;
};
