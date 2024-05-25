// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CollisionsCollectionModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCollisionsCollection, Log, All);

class FCollisionsCollectionModule : public ICollisionsCollectionModule {
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
