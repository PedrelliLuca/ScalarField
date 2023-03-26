// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ThermodynamicsModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogThermodynamics, Log, All);

class FThermodynamicsModule : public IThermodynamicsModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
