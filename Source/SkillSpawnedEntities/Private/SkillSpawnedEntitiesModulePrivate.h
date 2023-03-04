// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillSpawnedEntitiesModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillSpawnedEntities, Log, All);

class FSkillSpawnedEntitiesModule : public ISkillSpawnedEntitiesModule {
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
