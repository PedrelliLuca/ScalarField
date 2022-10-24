// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorTargetParameters.h"
#include "ActorSpawnerParameters.h"
#include "CoreMinimal.h"
#include "FollowerActorSpawnerParameters.h"

#include "SkillParameters.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct SKILLSYSTEM_API FSkillParameters {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double ManaCost = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double Cooldown = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double CastTime = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double ChannelingTime = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double Duration = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double BaseDamage = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes")
	bool DisablesMovementDuringCast = false;

	UPROPERTY(EditAnywhere, Category = "Actor Spawner Components")
	TArray<FActorSpawnerParameters> ActorSpawnerParameters;

	UPROPERTY(EditAnywhere, Category = "Follower Actor Spawner Components")
	TArray<FFollowerActorSpawnerParameters> FollowerActorSpawnerParameters;

	UPROPERTY(EditAnywhere, Category = "Actor Target Components")
	TArray<FActorTargetParameters> ActorTargetParameters;
};