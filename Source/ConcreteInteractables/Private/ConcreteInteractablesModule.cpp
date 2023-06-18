// Fill out your copyright notice in the Description page of Project Settings.

#include "ConcreteInteractablesModule.h"
#include "ConcreteInteractablesModulePrivate.h"

DEFINE_LOG_CATEGORY(LogConcreteInteractables);

IMPLEMENT_MODULE(FConcreteInteractablesModule, ConcreteInteractables);

void FConcreteInteractablesModule::StartupModule() {
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FConcreteInteractablesModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
