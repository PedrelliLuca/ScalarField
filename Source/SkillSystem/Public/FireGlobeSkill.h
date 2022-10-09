// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerSkill.h"
#include "GameFramework/SpringArmComponent.h"

#include "FireGlobeSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UFireGlobeSkill : public USpawnerSkill {
	GENERATED_BODY()
	
public:
	bool Cast(TObjectPtr<APawn> caster) override;
};
