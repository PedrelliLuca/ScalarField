// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AnimationComponentsModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAnimationComponents, Log, All);

class FAnimationComponentsModule : public IAnimationComponentsModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
