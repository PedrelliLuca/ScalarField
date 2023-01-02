// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "AbstractUIModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAbstractUI, Log, All);

class FAbstractUIModule : public IAbstractUIModule
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
