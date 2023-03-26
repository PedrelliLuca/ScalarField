// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayAttributesModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameplayAttributes, Log, All);

class FGameplayAttributesModule : public IGameplayAttributesModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
