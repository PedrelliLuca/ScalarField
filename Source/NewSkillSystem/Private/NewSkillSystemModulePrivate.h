// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NewSkillSystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogNewSkillSystem, Log, All);

class FNewSkillSystemModule : public INewSkillSystemModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
