// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillSystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillSystem, Log, All);

class FSkillSystemModule : public ISkillSystemModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
