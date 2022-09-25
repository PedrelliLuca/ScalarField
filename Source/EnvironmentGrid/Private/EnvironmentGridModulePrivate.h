// Copyright (c) 2014-2020 Zuru Tech HK Limited and Archbox S.r.l., All rights reserved.

#pragma once
#include "EnvironmentGridModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEnvironmentGrid, Log, All);

class FEnvironmentGridModule : public IEnvironmentGridModule
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};