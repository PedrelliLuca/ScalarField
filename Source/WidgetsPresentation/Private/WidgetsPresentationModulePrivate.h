// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetsPresentationModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWidgetsPresentation, Log, All);

class FWidgetsPresentationModule : public IWidgetsPresentationModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
