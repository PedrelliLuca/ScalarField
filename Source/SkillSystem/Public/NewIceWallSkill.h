// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractSkill.h"

#include "NewIceWallSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UNewIceWallSkill : public UAbstractSkill {
	GENERATED_BODY()
	
public:
	void Execute(TObjectPtr<AActor> caster) override;

#if DO_CHECK
	void CheckParametersSanity() const override;
#endif
};
