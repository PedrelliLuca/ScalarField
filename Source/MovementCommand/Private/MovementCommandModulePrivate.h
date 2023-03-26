// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MovementCommandModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMovementCommand, Log, All);

class FMovementCommandModule : public IMovementCommandModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
