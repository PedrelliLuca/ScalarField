// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillCastConditionsModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillCastConditions, Log, All);

class FSkillCastConditionsModule : public ISkillCastConditionsModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
