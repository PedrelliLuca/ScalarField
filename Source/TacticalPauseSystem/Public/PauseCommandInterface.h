// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PauseCommandInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UPauseCommand : public UInterface {
    GENERATED_BODY()
};

/* Interface representing an inventory. */
class IPauseCommand {
    GENERATED_BODY()

public:
    virtual void Execute() = 0;
};
