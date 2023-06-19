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
    const auto timeRatio = FMath::Clamp(_currentRotationTime / _requiredRotationTime, 0.0f, 1.0f);
    const auto rotationThisFrame = FMath::Lerp(_startRotation, _targetRotation, timeRatio);
    _opening->SetWorldRotation(rotationThisFrame);

    if (FMath::IsNearlyEqual(timeRatio, 1.0f)) {
        SetComponentTickEnabled(false);
        if (_openingState == EOpeningState::Opening) {
            _openingState = EOpeningState::Open;
        } else if (_openingState == EOpeningState::Closing) {
            _openingState = EOpeningState::Closed;
        } else {
            checkNoEntry();
        }
        _onOpeningStateChange.Broadcast(_openingState);
    }
}

void UOpeningsInteractionComponent::Toggle() {
    SetComponentTickEnabled(true);

    _startRotation = _opening->GetComponentRotation();
    switch (_openingState) {
        case EOpeningState::Closed:
            _targetRotation = _openingParameters.OpenRotation;
            _requiredRotationTime = _openingParameters.OpenCloseTime;
            _openingState = EOpeningState::Opening;
            break;
        case EOpeningState::Closing:
            _targetRotation = _openingParameters.OpenRotation;
            _requiredRotationTime = _currentRotationTime;
            _openingState = EOpeningState::Opening;
            break;
        case EOpeningState::Open:
            _targetRotation = _openingParameters.CloseRotation;
            _requiredRotationTime = _openingParameters.OpenCloseTime;
            _openingState = EOpeningState::Closing;
            break;
        case EOpeningState::Opening:
            _targetRotation = _openingParameters.CloseRotation;
            _requiredRotationTime = _currentRotationTime;
            _openingState = EOpeningState::Closing;
            break;
        default:
            checkNoEntry();
    }

    _onOpeningStateChange.Broadcast(_openingState);
    _currentRotationTime = 0.0f;
}

void UOpeningsInteractionComponent::SetOpening(TObjectPtr<UStaticMeshComponent> opening, const FOpeningParameters& openingParams) {
    if (!ensureMsgf(!_opening.IsValid(), TEXT("Since _opening has already been set, it won't be set again"))) {
        return;
    }

    _opening = opening;
    _openingParameters = openingParams;
}
