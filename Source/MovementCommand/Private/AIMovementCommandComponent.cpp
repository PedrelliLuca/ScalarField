// Fill out your copyright notice in the Description page of Project Settings.

#include "AIMovementCommandComponent.h"

void UAIMovementCommandComponent::SetMovementMode(EMovementCommandMode mode) {
    // Did you forget to set this up from the editor?
    check(_modesToCommandClasses.Contains(mode));
    if (mode == _activeMovementMode) {
        return;
    }

    // We should always enter except for the first call to SetMovementMode(), when the commands' cache is empty
    if (auto const activeCmdPtr = _modesToCommands.Find(_activeMovementMode)) {
        auto const activeCmd = *activeCmdPtr;
        activeCmd->OnStopMovement();
    }

    // In case we never entered this movement mode before, create its command and add it to the cache
    if (_modesToCommands.Find(mode) == nullptr) {
        auto const ownerAIC = Cast<AAIController>(GetOwner());
        if (ensureMsgf(IsValid(ownerAIC), TEXT("Error, owner is not an AI Controller!"))) {
            auto const& cmdClass = _modesToCommandClasses[mode];
            auto const newCmd = NewObject<UAIMovementCommand>(this, cmdClass);
            newCmd->SetAIController(ownerAIC);
            _modesToCommands.Emplace(mode, newCmd);
        }
    }

    _activeMovementMode = mode;
}

void UAIMovementCommandComponent::SetDestination(FVector const& destination) {
    _getMovementCommand()->OnSetDestination(destination);
}

void UAIMovementCommandComponent::StopMovement() {
    _getMovementCommand()->OnStopMovement();
}

void UAIMovementCommandComponent::MovementTick(float const deltaTime) {
    _getMovementCommand()->OnMovementTick(deltaTime);
}

void UAIMovementCommandComponent::SetMovementParameters(FMovementParameters const& params) {
    for (auto const& modeToCommand : _modesToCommands) {
        modeToCommand.Value->SetMovementParameters(params);
    }
}

TObjectPtr<UAIMovementCommand> UAIMovementCommandComponent::_getMovementCommand() {
    auto const activeCmd = _modesToCommands.Find(_activeMovementMode);
    // Did you set the movement mode before calling this?
    check(activeCmd != nullptr);
    return *activeCmd;
}
