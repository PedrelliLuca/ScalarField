// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "InventoryCoreModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryCore, Log, All);

class FInventoryCoreModule : public IInventoryCoreModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
