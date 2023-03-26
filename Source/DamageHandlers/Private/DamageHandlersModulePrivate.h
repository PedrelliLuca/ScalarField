// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DamageHandlersModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDamageHandlers, Log, All);

class FDamageHandlersModule : public IDamageHandlersModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
