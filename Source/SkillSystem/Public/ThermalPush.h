// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SpawnerSkill.h"

#include "ThermalPush.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UThermalPush : public USpawnerSkill, public FTickableGameObject {
	GENERATED_BODY()
	
public:
	bool CastSkill(TObjectPtr<APawn> caster) override;
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override;
	bool IsAllowedToTick() const override { return _spawnCapsule.IsValid(); }

private:
	TWeakObjectPtr<UCapsuleComponent> _spawnCapsule;

	// When the skill is casted with the caster having a temperature above this threshold, hit ignitable objects are set on fire
	UPROPERTY(EditAnywhere, Category = "Thresholds")
	double _hotThreshold = 300.;
	// When the skill is casted with the caster having a temperature below this threshold, hit freezable objects are frozen
	UPROPERTY(EditAnywhere, Category = "Thresholds")
	double _coldThreshold = 270.;

	UPROPERTY(EditAnywhere, Category = "Collision")
	double _minCapsuleHalfHeight = 10.;
	UPROPERTY(EditAnywhere, Category = "Collision")
	double _maxCapsuleHalfHeight = 200.;
	UPROPERTY(EditAnywhere, Category = "Collision")
	double _minCapsuleRadius = 5.;
	UPROPERTY(EditAnywhere, Category = "Collision")
	double _maxCapsuleRadius = 100.;

	UPROPERTY(EditAnywhere, Category = "Particles")
	TSubclassOf<UParticleSystemComponent> _hotParticleSystem = nullptr;
	UPROPERTY(EditAnywhere, Category = "Particles")
	TSubclassOf<UParticleSystemComponent> _coldParticleSystem = nullptr;

	double _timeFromCast = 0.;
};
