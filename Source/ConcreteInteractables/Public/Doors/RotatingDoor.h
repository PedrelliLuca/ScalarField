// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "InteractableComponent.h"
#include "InteractorInterface.h"
#include "OpeningsInteractionComponent.h"
#include "DoorKeyItem.h"

#include "RotatingDoor.generated.h"

UCLASS()
class CONCRETEINTERACTABLES_API ARotatingDoor : public AActor {
    GENERATED_BODY()

public:
    ARotatingDoor();

protected:
    void BeginPlay() override;

private:
    UFUNCTION()
    void _onDoorInteractedBy(TScriptInterface<IInteractor> interactor);

    UPROPERTY(EditAnywhere, Category = "Door Components")
    TObjectPtr<UStaticMeshComponent> _rotatingMesh = nullptr;

    UPROPERTY(EditAnywhere, Category = "Door Components")
    TObjectPtr<UInteractableComponent> _interactableC = nullptr;

    UPROPERTY(EditAnywhere, Category = "Door Components")
    TObjectPtr<UOpeningsInteractionComponent> _openingsC = nullptr;

    UPROPERTY(EditAnywhere, Category = "Door Parameters")
    float _timeToOpen = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Door Parameters")
    float _yawToOpen = 90.0f;

    UPROPERTY(EditAnywhere, Category = "Door Parameters")
    TSubclassOf<UDoorKeyItem> _compatibleKey = nullptr;
};
