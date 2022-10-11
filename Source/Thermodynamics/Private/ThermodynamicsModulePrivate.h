// Copyright (c) 2014-2021 Zuru Tech HK Limited, All rights reserved.

#pragma once
#include "ThermodynamicsModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogThermodynamics, Log, All);

class FThermodynamicsModule : public IThermodynamicsModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
