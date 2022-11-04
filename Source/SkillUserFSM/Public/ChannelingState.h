// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExecutionState.h"
#include "ManaComponent.h"

#include "ChannelingState.generated.h"


/**
 * \brief State representing a skill user that is casting a skill.
 * We define casting as that period of time where the character is preparing (cast time != 0) and then firing the skill. 
 * When we enter this state, we spend the mana needed to cast the skill.
 * Once casting is over, we transition to the channeling state if the skill requires it, otherwise we go to idle state.
 */
UCLASS(Blueprintable)
class SKILLUSERFSM_API UChannelingState : public UExecutionState {
	GENERATED_BODY()

public:
	TObjectPtr<USkillUserState> OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnBeginSkillExecution(int32 skillKey, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnTick(float deltaTime, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnSkillExecutionAborted(TObjectPtr<AController> controller) override;

	void OnEnter(TObjectPtr<AController> controller) override;
	void OnLeave(TObjectPtr<AController> controller) override;

private:
	template <typename S>
	TObjectPtr<S> _leaveChannelingForState(TObjectPtr<AController> controller) {
		static_assert(TIsDerivedFrom<S, USkillUserState>::Value, "S must derived from USkillUserState abstract class");
        static_assert(!TIsSame<S, USkillUserState>::Value, "S must not be of type USkillUserState");
		static_assert(!TIsSame<S, UExecutionState>::Value, "S must not be of type UExecutionState");

		const auto skill = GetSkillInExecution();
		skill->AbortChanneling();
		skill->RemoveAllTargets();
		return NewObject<S>(controller);
	}

	double _elapsedChannelingTime = 0.;

	TWeakObjectPtr<AActor> _caster = nullptr;
	TWeakObjectPtr<UManaComponent> _casterManaC = nullptr;
};
