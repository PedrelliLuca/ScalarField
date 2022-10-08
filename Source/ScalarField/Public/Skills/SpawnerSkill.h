// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/BaseSkill.h"
#include "SpawnerSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SCALARFIELD_API USpawnerSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	void Cast() override;

private:
	// TODO: in the future, the spawn location might work with a targeting system

	// Location in the spawning character's reference frame
	UPROPERTY(EditAnywhere, Category = "Skill Properties")
	FVector _spawnLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Skill Properties")
	TSubclassOf<AActor> _actorClass = nullptr;
};
