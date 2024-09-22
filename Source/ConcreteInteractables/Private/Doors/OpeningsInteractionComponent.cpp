// Fill out your copyright notice in the Description page of Project Settings.

#include "Doors/OpeningsInteractionComponent.h"

#include "Algo/ForEach.h"

UOpeningsInteractionComponent::UOpeningsInteractionComponent() {
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UOpeningsInteractionComponent::BeginPlay() {
    Super::BeginPlay();

    SetComponentTickEnabled(false);
}

void UOpeningsInteractionComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    Algo::ForEach(_openingToState, [this, deltaTime](auto& openingToState) {
        auto& opening = openingToState.Key;
        auto& openingState = openingToState.Value;

        if (openingState.Phase != EOpeningPhase::Opening && openingState.Phase != EOpeningPhase::Closing) {
            return;
        }

        openingState.CurrentRotationTime += deltaTime;
        auto const timeRatio = FMath::Clamp(openingState.CurrentRotationTime / openingState.RequiredRotationTime, 0.0f, 1.0f);
        auto const rotationThisFrame = FMath::Lerp(openingState.StartRotation, openingState.TargetRotation, timeRatio);
        opening->SetWorldRotation(rotationThisFrame);

        if (FMath::IsNearlyEqual(timeRatio, 1.0f)) {
            switch (openingState.Phase) {
                case EOpeningPhase::Opening:
                    openingState.Phase = EOpeningPhase::Open;
                    break;
                case EOpeningPhase::Closing:
                    openingState.Phase = EOpeningPhase::Closed;
                    break;
                default:
                    checkNoEntry();
            }

            if (++_numOpeningsThatEndedTransition == _openingToState.Num()) {
                switch (_overallOpeningPhase) {
                    case EOpeningPhase::Opening:
                        _overallOpeningPhase = EOpeningPhase::Open;
                        break;
                    case EOpeningPhase::Closing:
                        _overallOpeningPhase = EOpeningPhase::Closed;
                        break;
                    default:
                        checkNoEntry();
                }
                _onOpeningPhaseChange.Broadcast(_overallOpeningPhase);
                SetComponentTickEnabled(false);
            }
        }
    });
}

void UOpeningsInteractionComponent::Toggle() {
    SetComponentTickEnabled(true);

    Algo::ForEach(_openingToState, [](auto& openingToState) {
        auto& opening = openingToState.Key;
        auto& openingState = openingToState.Value;

        openingState.StartRotation = opening->GetComponentRotation();
        switch (openingState.Phase) {
            case EOpeningPhase::Closed:
                openingState.TargetRotation = openingState.Parameters.OpenRotation;
                openingState.RequiredRotationTime = openingState.Parameters.OpenCloseTime;
                openingState.Phase = EOpeningPhase::Opening;
                break;
            case EOpeningPhase::Closing:
                openingState.TargetRotation = openingState.Parameters.OpenRotation;
                openingState.RequiredRotationTime = openingState.CurrentRotationTime;
                openingState.Phase = EOpeningPhase::Opening;
                break;
            case EOpeningPhase::Open:
                openingState.TargetRotation = openingState.Parameters.CloseRotation;
                openingState.RequiredRotationTime = openingState.Parameters.OpenCloseTime;
                openingState.Phase = EOpeningPhase::Closing;
                break;
            case EOpeningPhase::Opening:
                openingState.TargetRotation = openingState.Parameters.CloseRotation;
                openingState.RequiredRotationTime = openingState.CurrentRotationTime;
                openingState.Phase = EOpeningPhase::Closing;
                break;
            default:
                checkNoEntry();
        }

        openingState.CurrentRotationTime = 0.0f;
    });

    switch (_overallOpeningPhase) {
        case EOpeningPhase::Closed:
        case EOpeningPhase::Closing:
            _overallOpeningPhase = EOpeningPhase::Opening;
            break;
        case EOpeningPhase::Open:
        case EOpeningPhase::Opening:
            _overallOpeningPhase = EOpeningPhase::Closing;
            break;
        default:
            checkNoEntry();
    }
    _onOpeningPhaseChange.Broadcast(_overallOpeningPhase);
    _numOpeningsThatEndedTransition = 0;
}

void UOpeningsInteractionComponent::SetOpening(TObjectPtr<UStaticMeshComponent> opening, FOpeningParameters const& openingParams) {
    if (!ensureMsgf(!_openingToState.Contains(opening), TEXT("Since _opening has already been set, it won't be set again"))) {
        return;
    }

    FOpeningState openingState;
    openingState.Parameters = openingParams;
    _openingToState.Emplace(opening, MoveTemp(openingState));
}
