// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractSkill.h"
#include "CoreMinimal.h"

#include "NewFireGlobeSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UNewFireGlobeSkill : public UAbstractSkill {
	GENERATED_BODY()
	
public:
	void Execute(TObjectPtr<AActor> caster) override;

#if DO_CHECK
	void CheckParametersSanity() const override;
#endif
};
