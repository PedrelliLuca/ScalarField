// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FactionSystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFactionSystem, Log, All);

class FFactionSystemModule : public IFactionSystemModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
