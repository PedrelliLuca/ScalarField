// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractSkill.h"

#include "CarbonizeSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UCarbonizeSkill : public UAbstractSkill {
	GENERATED_BODY()
	
public:
	void ExecuteCast(TObjectPtr<AActor> caster) override;

	bool IsValidTarget(int32 targetIndex, TObjectPtr<AActor> target) const override;

#if DO_CHECK
	void CheckParametersSanity() const override;
#endif

private:
	UPROPERTY(EditAnywhere, Category = "CarbonizeParameters")
	double _carbonizationTemperature = 900.;
};
