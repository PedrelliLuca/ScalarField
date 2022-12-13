// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExecutionState.h"
#include "ManaComponent.h"

#include "CastingState.generated.h"


/**
 * \brief State representing a skill user that is casting a skill.
 * We define casting as that period of time where the character is preparing (cast time != 0) and then firing the skill. 
 * When we enter this state, we spend the mana needed to cast the skill.
 * Once casting is over, we transition to the channeling state if the skill requires it, otherwise we go to idle state.
 */
UCLASS(Blueprintable)
class SKILLUSERFSM_API UCastingState : public UExecutionState {
	GENERATED_BODY()

public:
	TObjectPtr<USkillUserState> OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnBeginSkillExecution(int32 skillKey, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnTick(float deltaTime, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnAbort(TObjectPtr<AController> controller) override;

	void OnEnter(TObjectPtr<AController> controller) override;
	void OnLeave(TObjectPtr<AController> controller) override;

private:
	double _elapsedCastTime = 0.;
	double _manaLeftToPay = 0.;

	TObjectPtr<USkillUserState> _determineStateBasedOnSkillChanneling(TObjectPtr<UAbstractSkill> skill, TObjectPtr<AController> controller);
	
	TWeakObjectPtr<AActor> _caster = nullptr;
	TWeakObjectPtr<UManaComponent> _casterManaC = nullptr;
};
