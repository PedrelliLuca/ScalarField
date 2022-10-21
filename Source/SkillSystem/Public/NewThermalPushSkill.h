// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractSkill.h"
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "Particles/ParticleSystem.h"

#include "NewThermalPushSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UNewThermalPushSkill : public UAbstractSkill, public FTickableGameObject {
	GENERATED_BODY()
	
public:
	UNewThermalPushSkill();

	void Execute(TObjectPtr<AActor> caster) override;

	void Tick(float deltaTime) override;
	TStatId GetStatId() const override;
	bool IsAllowedToTick() const override { return _spawnCapsule.IsValid(); }

private:
	TWeakObjectPtr<UCapsuleComponent> _spawnCapsule;

	// When the skill is casted with the caster having a temperature above this threshold, hit ignitable objects are set on fire
	UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Thresholds")
	double _hotThreshold = 300.;
	// When the skill is casted with the caster having a temperature below this threshold, hit freezable objects are frozen
	UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Thresholds")
	double _coldThreshold = 270.;

	UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Collision")
	double _minCapsuleHalfHeight = 10.;
	UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Collision")
	double _maxCapsuleHalfHeight = 200.;
	UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Collision")
	double _minCapsuleRadius = 5.;
	UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Collision")
	double _maxCapsuleRadius = 100.;

	UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Particles")
	TObjectPtr<UParticleSystem> _hotTemplate;
	UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Particles")
	TObjectPtr<UParticleSystem> _coldTemplate;

	double _timeFromCast = 0.;
};
