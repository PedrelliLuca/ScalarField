// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ConcreteSkillsModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConcreteSkills, Log, All);

class FConcreteSkillsModule : public IConcreteSkillsModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
