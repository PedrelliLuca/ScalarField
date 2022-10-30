// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExecutionState.h"

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
	TObjectPtr<USkillUserState> OnSkillExecutionAborted(TObjectPtr<AController> controller) override;

	void OnEnter(TObjectPtr<AController> controller) override;
	void OnLeave(TObjectPtr<AController> controller) override;

	bool DisablesMovement() const override { return GetSkillInExecution()->DisablesMovementDuringCast(); }

private:
	void _endCasting();

	bool _bEndCasting = false;
	FTimerHandle _countdownToCast{};
	TWeakObjectPtr<AActor> _caster = nullptr;
};
