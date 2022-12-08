// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystemModule.h"
#include "InteractionSystemModulePrivate.h"

DEFINE_LOG_CATEGORY(LogInteractionSystem);

IMPLEMENT_MODULE(FInteractionSystemModule, InteractionSystem);

void FInteractionSystemModule::StartupModule() {
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FInteractionSystemModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
