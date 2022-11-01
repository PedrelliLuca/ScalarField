// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillUserFSMModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillUserFSM, Log, All);

class FSkillUserFSMModule : public ISkillUserFSMModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
