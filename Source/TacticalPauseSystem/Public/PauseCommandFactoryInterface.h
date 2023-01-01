// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PauseCommandInterface.h"

#include "PauseCommandFactoryInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UPauseCommandFactory : public UInterface {
	GENERATED_BODY()
};

/* Interface representing an inventory. */
class IPauseCommandFactory {
	GENERATED_BODY()

public:
	virtual TScriptInterface<IPauseCommand> CreateCommand() = 0;
};