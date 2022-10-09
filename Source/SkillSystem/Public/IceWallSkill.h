// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerSkill.h"
#include "IceWallSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UIceWallSkill : public USpawnerSkill {
	GENERATED_BODY()
	
public:
	bool Cast() override;

};
