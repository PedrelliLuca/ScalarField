// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "InteractionSystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInteractionSystem, Log, All);

class FInteractionSystemModule : public IInteractionSystemModule {
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
