// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "InventoryManipulationModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryManipulation, Log, All);

class FInventoryManipulationModule : public IInventoryManipulationModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
