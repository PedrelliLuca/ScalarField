// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingModule.h"
#include "SkillTargetingModulePrivate.h"

DEFINE_LOG_CATEGORY(LogSkillTargeting);

IMPLEMENT_MODULE(FSkillTargetingModule, SkillTargeting);

void FSkillTargetingModule::StartupModule() {
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FSkillTargetingModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
