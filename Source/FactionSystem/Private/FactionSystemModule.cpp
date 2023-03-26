// Fill out your copyright notice in the Description page of Project Settings.

#include "FactionSystemModule.h"
#include "FactionSystemModulePrivate.h"

DEFINE_LOG_CATEGORY(LogFactionSystem);

IMPLEMENT_MODULE(FFactionSystemModule, FactionSystem);

void FFactionSystemModule::StartupModule() {
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FFactionSystemModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
