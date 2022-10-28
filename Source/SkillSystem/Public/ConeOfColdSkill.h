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

#if DO_CHECK
	void CheckParametersSanity() const override;
#endif

private:
	UPROPERTY(EditAnywhere, Category = "CarbonizeParameters")
	double _carbonizationTemperature = 900.;
};
