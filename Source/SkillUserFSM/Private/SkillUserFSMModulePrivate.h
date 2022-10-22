// Copyright (c) 2014-2021 Zuru Tech HK Limited, All rights reserved.

#pragma once
#include "SkillUserFSMModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillUserFSM, Log, All);

class FSkillUserFSMModule : public ISkillUserFSMModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
