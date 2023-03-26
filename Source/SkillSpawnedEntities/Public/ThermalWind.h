// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "ImpactOneHitDamageDealerComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "SkillSpawnedEntityInterface.h"

#include "ThermalWind.generated.h"

UCLASS()
class SKILLSPAWNEDENTITIES_API AThermalWind : public AActor, public ISkillSpawnedEntity {
	GENERATED_BODY()
	
public:	
	AThermalWind();

	void Tick(float deltaTime) override;
	void SetCaster(TWeakObjectPtr<AActor> caster) override { _caster = caster; }

protected:
	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UImpactOneHitDamageDealerComponent> _impactOneHitDmgDealerC = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> _particleSystem = nullptr;

	// When the skill is casted with the caster having a temperature above this threshold, hit ignitable objects are set on fire
	UPROPERTY(EditAnywhere, Category = "Thermal Wind | Thresholds")
	double _hotThreshold = 300.;
	// // When the skill is casted with the caster having a temperature below this threshold, hit freezable objects are frozen
	UPROPERTY(EditAnywhere, Category = "Thermal Wind | Thresholds")
	double _coldThreshold = 270.;

	UPROPERTY(EditAnywhere, Category = "Thermal Wind | Particles")
	TObjectPtr<UParticleSystem> _hotTemplate;
	UPROPERTY(EditAnywhere, Category = "Thermal Wind | Particles")
	TObjectPtr<UParticleSystem> _coldTemplate;

	TWeakObjectPtr<AActor> _caster = nullptr;
};
