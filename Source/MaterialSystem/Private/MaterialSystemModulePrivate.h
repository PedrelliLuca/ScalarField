// Copyright (c) 2014-2021 Zuru Tech HK Limited, All rights reserved.

#pragma once
#include "MaterialSystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMaterialSystem, Log, All);

class FMaterialSystemModule : public IMaterialSystemModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
