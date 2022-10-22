// Copyright (c) 2014-2021 Zuru Tech HK Limited, All rights reserved.

#include "SkillUserFSMModule.h"
#include "SkillUserFSMModulePrivate.h"

DEFINE_LOG_CATEGORY(LogSkillUserFSM);

IMPLEMENT_MODULE(FSkillUserFSM Module, SkillUserFSM);

void FSkillUserFSMModule::StartupModule()
{
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FSkillUserFSMModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
