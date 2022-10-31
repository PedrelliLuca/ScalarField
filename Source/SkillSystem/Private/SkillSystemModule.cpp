// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillSystemModule.h"
#include "SkillSystemModulePrivate.h"

DEFINE_LOG_CATEGORY(LogSkillSystem);

IMPLEMENT_MODULE(FSkillSystemModule, SkillSystem);

void FSkillSystemModule::StartupModule()
{
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FSkillSystemModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
