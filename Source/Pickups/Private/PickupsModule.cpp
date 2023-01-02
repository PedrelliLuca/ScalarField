// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupsModule.h"
#include "PickupsModulePrivate.h"

DEFINE_LOG_CATEGORY(LogPickups);

IMPLEMENT_MODULE(FPickupsModule, Pickups);

void FPickupsModule::StartupModule()
{
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FPickupsModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
