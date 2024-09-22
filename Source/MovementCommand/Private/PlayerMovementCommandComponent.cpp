// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMovementCommandComponent.h"

void UPlayerMovementCommandComponent::SetMovementMode(EMovementCommandMode mode) {
    // Did you forget to set this up from the editor?
    check(_modesToCommandClasses.Contains(mode));
    if (mode == _activeMovementMode) {
        return;
    }

    _activeMovementMode = mode;

    auto const& cmdClass = _modesToCommandClasses[_activeMovementMode];
    _activeMovementCommand = NewObject<UPlayerMovementCommand>(this, cmdClass);
}

void UPlayerMovementCommandComponent::SetDestination(FVector const& destination) {
    check(_ownerPC.IsValid());
    check(_inputData.IsSet());

    // Point of failure: don't forget to call Reset() on _inputData, it must be set again before calling SetDestination()
    _getMovementCommand()->OnSetDestination(_ownerPC.Get(), destination, _inputData.GetValue());
    _inputData.Reset();
}

void UPlayerMovementCommandComponent::StopMovement() {
    check(_ownerPC.IsValid());
    _getMovementCommand()->OnStopMovement(_ownerPC.Get());
}

void UPlayerMovementCommandComponent::MovementTick(float const deltaTime) {
    check(_ownerPC.IsValid());
    _getMovementCommand()->OnMovementTick(_ownerPC.Get(), deltaTime);
}

void UPlayerMovementCommandComponent::SetPlayerInputData(FPlayerInputData&& inputData) {
    _inputData = MoveTemp(inputData);
}

void UPlayerMovementCommandComponent::BeginPlay() {
    Super::BeginPlay();

    _ownerPC = Cast<APlayerController>(GetOwner());
    ensureMsgf(_ownerPC.IsValid(), TEXT("Error, owner is not a Player Controller!"));
}

TObjectPtr<UPlayerMovementCommand> UPlayerMovementCommandComponent::_getMovementCommand() {
    // Did you set the movement mode before calling this?
    check(IsValid(_activeMovementCommand));
    return _activeMovementCommand;
}
