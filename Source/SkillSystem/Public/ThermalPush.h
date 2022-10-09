// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "SpawnerSkill.h"

#include "ThermalPush.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UThermalPush : public USpawnerSkill {
	GENERATED_BODY()
	
public:
	bool Cast(TObjectPtr<APawn> caster) override;

private:
	// When the skill is casted with the caster having a temperature above this threshold, hit ignitable objects are set on fire
	UPROPERTY(EditAnywhere, Category = "Thresholds")
	double _hotThreshold = 300.;
	// When the skill is casted with the caster having a temperature below this threshold, hit freezable objects are frozen
	UPROPERTY(EditAnywhere, Category = "Thresholds")
	double _coldThreshold = 270.;


	UPROPERTY(EditAnywhere, Category = "Particles")
	TSubclassOf<UParticleSystemComponent> _hotParticleSystem = nullptr;
	UPROPERTY(EditAnywhere, Category = "Particles")
	TSubclassOf<UParticleSystemComponent> _coldParticleSystem = nullptr;
};
