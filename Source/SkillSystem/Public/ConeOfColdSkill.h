// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractSkill.h"

#include "ConeOfColdSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UConeOfColdSkill : public UAbstractSkill {
	GENERATED_BODY()
	
public:
	void ExecuteCast(TObjectPtr<AActor> caster) override;
	void ExecuteChannelingTick(float deltaTime, const TObjectPtr<AActor> caster) override;
	void AbortChanneling() override;

#if DO_CHECK
	void CheckParametersSanity() const override;
#endif

private:
	TWeakObjectPtr<UCapsuleComponent> _cone;
	TWeakObjectPtr<AController> _casterController;

	UPROPERTY(EditAnywhere, Category = "ConeOfColdParameters")
	double _coneTemperature = 0.;
	UPROPERTY(EditAnywhere, Category = "ConeOfColdParameters")
	double _height = 300.;
	UPROPERTY(EditAnywhere, Category = "ConeOfColdParameters")
	double _radius = 50.;
};
