// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Skill.generated.h"

UINTERFACE(MinimalAPI)
class USkill : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class SCALARFIELD_API ISkill {
	GENERATED_BODY()

public:
	virtual bool Cast() PURE_VIRTUAL(ISkill::Cast, return false;);
	virtual double GetManaCost() const PURE_VIRTUAL(ISkill::GetManaCost, return 0.;);
};
