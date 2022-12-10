// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "InventorySystemModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventorySystem, Log, All);

class FInventorySystemModule : public IInventorySystemModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
