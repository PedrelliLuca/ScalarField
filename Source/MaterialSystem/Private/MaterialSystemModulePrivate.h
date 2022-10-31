// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MaterialSystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMaterialSystem, Log, All);

class FMaterialSystemModule : public IMaterialSystemModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
