// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ConcreteItemsModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConcreteItems, Log, All);

class FConcreteItemsModule : public IConcreteItemsModule {
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
