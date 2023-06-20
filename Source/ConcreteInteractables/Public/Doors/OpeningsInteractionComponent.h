// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "OpeningsInteractionComponent.generated.h"

struct FOpeningParameters {
    float OpenCloseTime = 0.0f;
    FRotator OpenRotation = FRotator::ZeroRotator;
    FRotator CloseRotation = FRotator::ZeroRotator;
};

enum EOpeningPhase
{
    None,
    Closed,
    Open,
    Closing,
    Opening,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOpeningPhaseChange, EOpeningPhase);

UCLASS()
class CONCRETEINTERACTABLES_API UOpeningsInteractionComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UOpeningsInteractionComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
    void Toggle();
    void SetOpening(TObjectPtr<UStaticMeshComponent> opening, const FOpeningParameters& openingParams);
    FOnOpeningPhaseChange& OnOpeningStateChange() { return _onOpeningPhaseChange; }

protected:
    void BeginPlay() override;

private:
    struct FOpeningState {
        float CurrentRotationTime = 0.0f;
        float RequiredRotationTime = 0.0f;
        FRotator StartRotation = FRotator::ZeroRotator;
        FRotator TargetRotation = FRotator::ZeroRotator;
        EOpeningPhase Phase = EOpeningPhase::Closed;
        FOpeningParameters Parameters;
    };

    TMap<TWeakObjectPtr<UStaticMeshComponent>, FOpeningState> _openingToState;
    EOpeningPhase _overallOpeningPhase = EOpeningPhase::Closed;
    FOnOpeningPhaseChange _onOpeningPhaseChange;
    int32 _numOpeningsThatEndedTransition = 0;
};
