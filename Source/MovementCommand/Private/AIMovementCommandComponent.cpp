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

		// By removing before stopping movement, we avoid weird situations where we would broadcast 2 times consecutively, one for
		// the stop of the old command and one for the activation of the new one. This way, we just broadcast the new command's activation.
		// This might have unforseen consequences in the future, in which case you should move the removal after the OnStopMovement() call.
		activeCmd->OnMovementStateChanged().Remove(_handleToCmdMovementStateChanged);
		activeCmd->OnStopMovement(_ownerAIController.Get());
	}

	// In case we never entered this movement mode before, create its command and add it to the cache
	if (_modesToCommands.Find(mode) == nullptr) {
		const auto& cmdClass = _modesToCommandClasses[mode];
		const auto newCmd = NewObject<UAIMovementCommand>(this, cmdClass);
		_modesToCommands.Emplace(mode, newCmd);
	}
	
	_activeMovementMode = mode;
	_handleToCmdMovementStateChanged = _modesToCommands[_activeMovementMode]->OnMovementStateChanged().AddUObject(this, &UAIMovementCommandComponent::_onActiveMovementCmdStatusChange);
}

void UAIMovementCommandComponent::BeginPlay() {
	Super::BeginPlay();

	_ownerAIController = Cast<AAIController>(GetOwner());
	check(_ownerAIController.IsValid());
}

void UAIMovementCommandComponent::_onActiveMovementCmdStatusChange(const bool newIsMoving) {
	_onActiveMovementCmdStateChanged.Broadcast(newIsMoving);
}
