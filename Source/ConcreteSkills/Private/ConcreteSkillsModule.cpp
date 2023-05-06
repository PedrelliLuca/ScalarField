// Fill out your copyright notice in the Description page of Project Settings.

#include "ConcreteSkillsModule.h"
#include "ConcreteSkillsModulePrivate.h"

DEFINE_LOG_CATEGORY(LogConcreteSkills);

IMPLEMENT_MODULE(FConcreteSkillsModule, ConcreteSkills);

void FConcreteSkillsModule::StartupModule() {
    // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FConcreteSkillsModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}
