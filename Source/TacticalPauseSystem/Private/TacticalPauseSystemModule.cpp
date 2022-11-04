// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalPauseSystemModule.h"
#include "TacticalPauseSystemModulePrivate.h"

DEFINE_LOG_CATEGORY(LogTacticalPauseSystem);

IMPLEMENT_MODULE(FTacticalPauseSystemModule, TacticalPauseSystem);

void FTacticalPauseSystemModule::StartupModule()
{
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FTacticalPauseSystemModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
