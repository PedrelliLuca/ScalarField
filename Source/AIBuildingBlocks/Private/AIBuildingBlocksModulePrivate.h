// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIBuildingBlocksModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAIBuildingBlocks, Log, All);

class FAIBuildingBlocksModule : public IAIBuildingBlocksModule {
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
