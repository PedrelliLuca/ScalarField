// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ThermodynamicsPresenterModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogThermodynamicsPresenter, Log, All);

class FThermodynamicsPresenterModule : public IThermodynamicsPresenterModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
