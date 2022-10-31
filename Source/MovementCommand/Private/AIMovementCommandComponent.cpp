// Fill out your copyright notice in the Description page of Project Settings.

#include "AIMovementCommandComponent.h"

void UAIMovementCommandComponent::SetMovementMode(EMovementCommandMode mode) {
	// Did you forget to set this up?
	check(_modesToCommandClasses.Contains(mode));
	_activeMovementMode = mode;

	const auto& cmdClass = _modesToCommandClasses[_activeMovementMode];
	_activeMovementCommand = NewObject<UAIMovementCommand>(this, cmdClass);
}
