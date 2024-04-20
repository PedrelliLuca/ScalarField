// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ThermodynamicsCoreModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogThermodynamicsCore, Log, All);

class FThermodynamicsCoreModule : public IThermodynamicsCoreModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
