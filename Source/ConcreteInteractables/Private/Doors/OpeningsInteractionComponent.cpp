// Fill out your copyright notice in the Description page of Project Settings.

#include "Doors/OpeningsInteractionComponent.h"

UOpeningsInteractionComponent::UOpeningsInteractionComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UOpeningsInteractionComponent::BeginPlay() {
    Super::BeginPlay();

    SetComponentTickEnabled(false);
}

void UOpeningsInteractionComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    _currentRotationTime += deltaTime;
    const auto timeRatio = FMath::Clamp(_currentRotationTime / _timeToRotate, 0.0f, 1.0f);
    const auto rotationThisFrame = FMath::Lerp(_closedRotation, _openRotation, timeRatio);
    _opening->SetWorldRotation(rotationThisFrame);

    if (FMath::IsNearlyEqual(timeRatio, 1.0f)) {
        SetComponentTickEnabled(false);
    }
}

void UOpeningsInteractionComponent::Open() {
    SetComponentTickEnabled(true);
}

void UOpeningsInteractionComponent::SetOpening(TObjectPtr<UStaticMeshComponent> opening, const FOpeningInteractionParameters& interactionParams) {
    if (!ensureMsgf(!_opening.IsValid(), TEXT("Since _opening has already been set, it won't be set again"))) {
        return;
    }

    _opening = opening;
    _closedRotation = opening->GetComponentRotation();
    _openRotation = _closedRotation + interactionParams.AmountToRotate;
    _timeToRotate = interactionParams.TimeToRotate;
    _currentRotationTime = 0.0f;
}
