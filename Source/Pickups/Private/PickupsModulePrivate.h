// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PickupsModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPickups, Log, All);

class FPickupsModule : public IPickupsModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
