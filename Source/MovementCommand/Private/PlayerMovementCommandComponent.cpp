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
