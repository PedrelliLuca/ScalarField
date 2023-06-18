// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ConcreteInteractablesModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConcreteInteractables, Log, All);

class FConcreteInteractablesModule : public IConcreteInteractablesModule {
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
