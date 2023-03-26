// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "InventoryPresenterModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryPresenter, Log, All);

class FInventoryPresenterModule : public IInventoryPresenterModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
