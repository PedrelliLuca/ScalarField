// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorSpawnerParameters.h"
#include "CapsuleSpawnerParameters.h"
#include "CoreMinimal.h"

#include "NewSkillParameters.generated.h"

UENUM()
enum class EConcreteSkill : uint32 {
	CS_Invalid = 0     UMETA(DisplayName = "Invalid"),
	CS_IceWall = 1     UMETA(DisplayName = "Ice Wall"),
	CS_FireGlobe = 2   UMETA(DisplayName = "Fire Globe"),
	CS_ThermalPush = 3 UMETA(DisplayName = "Thermal Push"),
};

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UNewSkillParameters : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Associated Skill")
	EConcreteSkill Skill = EConcreteSkill::CS_Invalid;

	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double ManaCost = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double Cooldown = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double CastTime = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double Duration = 0.;
	UPROPERTY(EditAnywhere, Category = "Basic Attributes", meta = (ClampMin = "0"))
	double BaseDamage = 0.;
	
	UPROPERTY(EditAnywhere, Category = "Capsule Components")
	TArray<FCapsuleSpawnerParameters> CapsuleSpawnerParameters;

	UPROPERTY(EditAnywhere, Category = "Actor Spawner Components")
	TArray<FActorSpawnerParameters> ActorSpawnerParameters;
};