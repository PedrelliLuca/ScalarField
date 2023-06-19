// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "OpeningsInteractionComponent.generated.h"

struct FOpeningParameters {
    float OpenCloseTime;
    FRotator OpenRotation;
    FRotator CloseRotation;
};

enum EOpeningState
{
    None,
    Closed,
    Open,
    Closing,
    Opening,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOpeningStateChange, EOpeningState);

UCLASS()
class CONCRETEINTERACTABLES_API UOpeningsInteractionComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UOpeningsInteractionComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
    void Toggle();
    void SetOpening(TObjectPtr<UStaticMeshComponent> opening, const FOpeningParameters& openingParams);
    FOnOpeningStateChange& OnOpeningStateChange() { return _onOpeningStateChange; }

protected:
    void BeginPlay() override;

private:
    TWeakObjectPtr<UStaticMeshComponent> _opening;

    float _currentRotationTime;
    float _requiredRotationTime;
    FRotator _startRotation;
    FRotator _targetRotation;
    EOpeningState _openingState = EOpeningState::Closed;
    FOpeningParameters _openingParameters;
    FOnOpeningStateChange _onOpeningStateChange;
};
