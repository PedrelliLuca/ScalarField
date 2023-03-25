// Fill out your copyright notice in the Description page of Project Settings.

#include "AIMovementCommandComponent.h"

void UAIMovementCommandComponent::SetMovementMode(EMovementCommandMode mode) {
	// Did you forget to set this up from the editor?
	check(_modesToCommandClasses.Contains(mode));
	if (mode == _activeMovementMode) {
		return;
	}

	// We should always enter except for the first call to SetMovementMode(), when the commands' cache is empty
	if (const auto activeCmd = _modesToCommands.Find(_activeMovementMode)) {
		(*activeCmd)->OnStopMovement(_ownerAIController.Get());
	}

	// In case we never entered this movement mode before, create its command and add it to the cache
	if (_modesToCommands.Find(mode) == nullptr) {
		const auto& cmdClass = _modesToCommandClasses[_activeMovementMode];
		const auto newCmd = NewObject<UAIMovementCommand>(this, cmdClass);
		_modesToCommands.Emplace(mode, newCmd);
	}
	
	_activeMovementMode = mode;
}

void UAIMovementCommandComponent::BeginPlay() {
	Super::BeginPlay();

	_ownerAIController = Cast<AAIController>(GetOwner());
	check(_ownerAIController.IsValid());
}
