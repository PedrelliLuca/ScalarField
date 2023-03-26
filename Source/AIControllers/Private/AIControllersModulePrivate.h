// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIControllersModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAIControllers, Log, All);

class FAIControllersModule : public IAIControllersModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
