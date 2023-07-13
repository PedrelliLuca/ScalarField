// Fill out your copyright notice in the Description page of Project Settings.

#include "AIMovementCommandComponent.h"

void UAIMovementCommandComponent::SetMovementMode(EMovementCommandMode mode) {
    // Did you forget to set this up from the editor?
    check(_modesToCommandClasses.Contains(mode));
    if (mode == _activeMovementMode) {
        return;
    }

    // We should always enter except for the first call to SetMovementMode(), when the commands' cache is empty
    if (const auto activeCmdPtr = _modesToCommands.Find(_activeMovementMode)) {
        const auto activeCmd = *activeCmdPtr;
        activeCmd->OnStopMovement();
    }

    // In case we never entered this movement mode before, create its command and add it to the cache
    if (_modesToCommands.Find(mode) == nullptr) {
        const auto ownerAIC = Cast<AAIController>(GetOwner());
        if (ensureMsgf(IsValid(ownerAIC), TEXT("Error, owner is not an AI Controller!"))) {
            const auto& cmdClass = _modesToCommandClasses[mode];
            const auto newCmd = NewObject<UAIMovementCommand>(this, cmdClass);
            newCmd->SetAIController(ownerAIC);
            _modesToCommands.Emplace(mode, newCmd);
        }
    }

    _activeMovementMode = mode;
}

void UAIMovementCommandComponent::SetDestination(const FVector& destination) {
    _getMovementCommand()->OnSetDestination(destination);
}

void UAIMovementCommandComponent::StopMovement() {
    _getMovementCommand()->OnStopMovement();
}

void UAIMovementCommandComponent::MovementTick(const float deltaTime) {
    _getMovementCommand()->OnMovementTick(deltaTime);
}

void UAIMovementCommandComponent::SetMovementParameters(const FMovementParameters& params) {
    for (const auto& modeToCommand : _modesToCommands) {
        modeToCommand.Value->SetMovementParameters(params);
    }
}

TObjectPtr<UAIMovementCommand> UAIMovementCommandComponent::_getMovementCommand() {
    const auto activeCmd = _modesToCommands.Find(_activeMovementMode);
    // Did you set the movement mode before calling this?
    check(activeCmd != nullptr);
    return *activeCmd;
}
