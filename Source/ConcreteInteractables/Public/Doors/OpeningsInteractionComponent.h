// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "OpeningsInteractionComponent.generated.h"

struct FOpeningInteractionParameters {
    float TimeToRotate;
    FRotator AmountToRotate;
};

UCLASS()
class CONCRETEINTERACTABLES_API UOpeningsInteractionComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UOpeningsInteractionComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
    void Open();
    void SetOpening(TObjectPtr<UStaticMeshComponent> opening, const FOpeningInteractionParameters& interactionParams);

protected:
    void BeginPlay() override;

private:
    TWeakObjectPtr<UStaticMeshComponent> _opening;

    FRotator _closedRotation;
    FRotator _openRotation;
    float _timeToRotate;
    float _currentRotationTime;
};
