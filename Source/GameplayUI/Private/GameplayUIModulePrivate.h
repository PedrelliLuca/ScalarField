// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayUIModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameplayUI, Log, All);

class FGameplayUIModule : public IGameplayUIModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
