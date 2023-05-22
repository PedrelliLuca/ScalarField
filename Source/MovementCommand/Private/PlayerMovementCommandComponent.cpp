// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMovementCommandComponent.h"

void UPlayerMovementCommandComponent::SetMovementMode(EMovementCommandMode mode) {
    // Did you forget to set this up from the editor?
    check(_modesToCommandClasses.Contains(mode));
    if (mode == _activeMovementMode) {
        return;
    }

    _activeMovementMode = mode;

    const auto& cmdClass = _modesToCommandClasses[_activeMovementMode];
    _activeMovementCommand = NewObject<UPlayerMovementCommand>(this, cmdClass);
}

void UPlayerMovementCommandComponent::SetDestination(const FVector& destination) {
    check(_ownerPC.IsValid());
    _getMovementCommand()->OnSetDestination(_ownerPC, destination);
}

void UPlayerMovementCommandComponent::StopMovement() {
    check(_ownerPC.IsValid());
    _getMovementCommand()->OnStopMovement(_ownerPC.Get());
}

void UPlayerMovementCommandComponent::MovementTick(const float deltaTime) {
    check(_ownerPC.IsValid());
    _getMovementCommand()->OnMovementTick(_ownerPC.Get(), deltaTime);
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
