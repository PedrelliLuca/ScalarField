// Copyright (c) 2014-2021 Zuru Tech HK Limited, All rights reserved.

#pragma once
#include "SkillSystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillSystem, Log, All);

class FSkillSystemModule : public ISkillSystemModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
