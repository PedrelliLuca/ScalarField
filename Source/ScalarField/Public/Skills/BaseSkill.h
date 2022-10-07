// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "UObject/NoExportTypes.h"

#include "BaseSkill.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class SCALARFIELD_API UBaseSkill : public UObject, public ISkill {
	GENERATED_BODY()

public:

private:
	UPROPERTY(EditAnywhere, Category = "Skill Properties")
	double _manaCost = 0.;
	UPROPERTY(EditAnywhere, Category = "Skill Properties", meta = (ClampMin = "0"))
	double _coolDown = 0.;
	UPROPERTY(EditAnywhere, Category = "Skill Properties", meta = (ClampMin = "0"))
	double _castTime = 0.;
	UPROPERTY(EditAnywhere, Category = "Skill Properties", meta = (ClampMin = "-1"))
	double _duration = 0.;
	UPROPERTY(EditAnywhere, Category = "Skill Properties", meta = (ClampMin = "0"))
	double _baseDamage = 0.;
	// TODO: add icon property
};
