// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "GameFramework/SpringArmComponent.h"

#include "ThermalPushSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UThermalPushSkill : public UAbstractSkill {
	GENERATED_BODY()
	
public:
	void ExecuteCast(TObjectPtr<AActor> caster) override;
	void Abort() override;

private:
	void _cleanupCallback();
	
	FTimerHandle _timerHandle;

	TWeakObjectPtr<USpringArmComponent> _spawnSpringArm = nullptr;
	TWeakObjectPtr<AActor> _spawnActor = nullptr;

	// When the skill is casted with the caster having a temperature above this threshold, hit ignitable objects are set on fire
	// UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Thresholds")
	// double _hotThreshold = 300.;
	// // When the skill is casted with the caster having a temperature below this threshold, hit freezable objects are frozen
	// UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Thresholds")
	// double _coldThreshold = 270.;
	//
	// UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Collision")
	// double _minCapsuleHalfHeight = 10.;
	// UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Collision")
	// double _maxCapsuleHalfHeight = 200.;
	// UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Collision")
	// double _minCapsuleRadius = 5.;
	// UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Collision")
	// double _maxCapsuleRadius = 100.;
	//
	// UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Particles")
	// TObjectPtr<UParticleSystem> _hotTemplate;
	// UPROPERTY(EditAnywhere, Category = "ThermalPushParameters | Particles")
	// TObjectPtr<UParticleSystem> _coldTemplate;
	//
	// TWeakObjectPtr<UParticleSystemComponent> _activeParticleSystem = nullptr;
	// TWeakObjectPtr<UCapsuleComponent> _pushCapsule;
	//
	// double _timeFromCast = 0.;
};
