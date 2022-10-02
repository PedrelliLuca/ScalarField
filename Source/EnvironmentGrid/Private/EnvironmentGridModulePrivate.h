// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EnvironmentGridModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEnvironmentGrid, Log, All);

class FEnvironmentGridModule : public IEnvironmentGridModule
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};