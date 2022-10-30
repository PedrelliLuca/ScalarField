// Copyright (c) 2014-2021 Zuru Tech HK Limited, All rights reserved.

#pragma once
#include "MovementCommandModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMovementCommand, Log, All);

class FMovementCommandModule : public IMovementCommandModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
