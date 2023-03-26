// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TacticalPauseSystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTacticalPauseSystem, Log, All);

class FTacticalPauseSystemModule : public ITacticalPauseSystemModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
