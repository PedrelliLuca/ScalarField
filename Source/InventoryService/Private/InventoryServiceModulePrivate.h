// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "InventoryServiceModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryService, Log, All);

class FInventoryServiceModule : public IInventoryServiceModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
