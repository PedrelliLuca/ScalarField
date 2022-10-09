// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "UObject/NoExportTypes.h"

#include "BaseSkill.generated.h"

USTRUCT(BlueprintType)
struct SCALARFIELD_API FSkillParameters {
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double ManaCost = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double CoolDown = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double CastTime = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double Duration = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double BaseDamage = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	TSubclassOf<UBaseSkill> Class;

	// TODO: add icon property
};

/**
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class SCALARFIELD_API UBaseSkill : public UObject, public ISkill {
	GENERATED_BODY()

public:
	const FSkillParameters& GetParameters() const { return _parameters; }
	void SetParameters(const FSkillParameters& parameters) { _parameters = parameters; }

private:
	FSkillParameters _parameters;
};
