// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractorInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UInteractor : public UInterface {
    GENERATED_BODY()
};

class IInteractor {
    GENERATED_BODY()

public:
    // Focus functions
    virtual void PerformFocusCheck() = 0;

    // Interaction functions
    virtual bool PerformInteractionCheck() = 0;
    UFUNCTION(BlueprintCallable)
    virtual void EndInteraction() = 0;
    virtual bool IsInteracting() const = 0;
    virtual double GetTimeLeftBeforeInteraction() const = 0;

private:
    // Focus functions
    virtual void _endFocus() = 0;

    // Interaction functions
    virtual void _interact() = 0;
};
