// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillUserState.generated.h"


/**
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class SKILLUSERFSM_API USkillUserState : public UObject {
	GENERATED_BODY()

public:
	virtual TObjectPtr<USkillUserState> OnTargeting(TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::OnTargeting, return nullptr;);
	virtual TObjectPtr<USkillUserState> OnBeginSkillExecution(int32 skillKey, TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::OnBeginSkillExecution, return nullptr;);
	virtual TObjectPtr<USkillUserState> OnTick(float deltaTime, TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::Tick, return nullptr;);

	virtual void OnEnter(TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::OnEnter, return;);
	virtual void OnLeave(TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::OnLeave, return;);

	virtual bool DisablesMovement() const PURE_VIRTUAL(USkillUserState::OnLMBPress, return false;);
	
protected:
	TObjectPtr<USkillUserState> _keepCurrentState() { 
		// The skill user must always be in a valid state. Returning nullptr does not invalid the state, but
		// rather signals that the user must not change its state.
		return nullptr;  
	}

	static constexpr uint32 KEY_ASSIGNABLE_SKILLS = 10;
};
