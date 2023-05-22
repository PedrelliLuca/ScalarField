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
        activeCmd->OnStopMovement(_ownerAIController.Get());
        activeCmd->OnMovementStateChanged().Remove(_handleToCmdMovementStateChanged);
    }

    // In case we never entered this movement mode before, create its command and add it to the cache
    if (_modesToCommands.Find(mode) == nullptr) {
        const auto& cmdClass = _modesToCommandClasses[mode];
        const auto newCmd = NewObject<UAIMovementCommand>(this, cmdClass);
        _modesToCommands.Emplace(mode, newCmd);
    }

    _activeMovementMode = mode;
    _handleToCmdMovementStateChanged =
        _modesToCommands[_activeMovementMode]->OnMovementStateChanged().AddUObject(this, &UAIMovementCommandComponent::_onActiveMovementCmdStatusChange);
}

void UAIMovementCommandComponent::SetMovementParameters(const FMovementParameters& params) {
    for (const auto& modeToCommand : _modesToCommands) {
        modeToCommand.Value->SetMovementParameters(params);
    }
}

void UAIMovementCommandComponent::BeginPlay() {
    Super::BeginPlay();

    _ownerAIController = Cast<AAIController>(GetOwner());
    check(_ownerAIController.IsValid());
}

TObjectPtr<UAIMovementCommand> UAIMovementCommandComponent::_getMovementCommand() {
    const auto activeCmd = _modesToCommands.Find(_activeMovementMode);
    // Did you set the movement mode before calling this?
    check(activeCmd != nullptr);
    return *activeCmd;
}

void UAIMovementCommandComponent::_onActiveMovementCmdStatusChange(const bool newIsMoving) {
    _onActiveMovementCmdStateChanged.Broadcast(newIsMoving);
}
