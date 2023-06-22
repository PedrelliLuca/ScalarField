// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DoorKeyItem.h"
#include "GameFramework/Actor.h"
#include "InteractableComponent.h"
#include "InteractorInterface.h"
#include "OpeningsInteractionComponent.h"

#include "DoubleOpeningDoor.generated.h"

UCLASS()
class CONCRETEINTERACTABLES_API ADoubleOpeningDoor : public AActor {
    GENERATED_BODY()

public:
    ADoubleOpeningDoor();

protected:
    void BeginPlay() override;

private:
    UFUNCTION()
    void _onDoorInteractedBy(TScriptInterface<IInteractor> interactor);

    void _onOpeningPhaseChange(EOpeningPhase newOpeningState);

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> _sceneC = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Door Components")
    TObjectPtr<UStaticMeshComponent> _leftOpeningMeshC = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Door Components")
    TObjectPtr<UStaticMeshComponent> _rightOpeningMeshC = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Door Components")
    TObjectPtr<UInteractableComponent> _interactableC = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Door Components")
    TObjectPtr<UOpeningsInteractionComponent> _openingsC = nullptr;

    UPROPERTY(EditAnywhere, Category = "Door Parameters")
    FOpeningParameters _leftOpeningParameters{};

    UPROPERTY(EditAnywhere, Category = "Door Parameters")
    FOpeningParameters _rightOpeningParameters{};

    UPROPERTY(EditAnywhere, Category = "Door Parameters")
    TSubclassOf<UDoorKeyItem> _compatibleKey = nullptr;
};
