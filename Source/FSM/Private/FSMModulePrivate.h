// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FSMModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFSM, Log, All);

class FFSMModule : public IFSMModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
