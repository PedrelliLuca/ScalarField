// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillInspectionModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillInspection, Log, All);

class FSkillInspectionModule : public ISkillInspectionModule {
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
