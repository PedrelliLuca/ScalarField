// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DamageDealersModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDamageDealers, Log, All);

class FDamageDealersModule : public IDamageDealersModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
