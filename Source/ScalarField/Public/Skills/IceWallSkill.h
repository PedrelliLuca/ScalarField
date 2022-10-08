// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SpawnerSkill.h"
#include "IceWallSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SCALARFIELD_API UIceWallSkill : public USpawnerSkill {
	GENERATED_BODY()
	
public:
	void Cast() override;

};
