// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillTargetingModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillTargeting, Log, All);

class FSkillTargetingModule : public ISkillTargetingModule {
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
