// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractionComponent.h"
#include "InteractorInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractorInterface : public UInterface {
	GENERATED_BODY()
};

class IInteractorInterface {
	GENERATED_BODY()
	
private:
	// Focus functions
	virtual void _performFocusCheck() = 0;
	virtual void _setInteractableBeingFocused(TWeakObjectPtr<UInteractionComponent>&& newInteractionComponent) = 0;
	virtual void _couldntFindInteractableToFocus() = 0;

	// Interaction functions
	virtual void _beginInteraction() = 0;
	virtual void _interact() = 0;
	virtual void _endInteraction() = 0;
};
