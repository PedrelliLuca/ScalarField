// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractorInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UInteractorInterface : public UInterface {
	GENERATED_BODY()
};

class IInteractorInterface {
	GENERATED_BODY()

public:
	virtual bool IsInteracting() const = 0;
	virtual double GetRemainingInteractionTime() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual void EndInteraction() = 0;
	
private:
	// Focus functions
	virtual void _performFocusCheck() = 0;
	virtual void _endFocus() = 0;

	// Interaction functions
	virtual void _beginInteraction() = 0;
	virtual void _interact() = 0;
};
