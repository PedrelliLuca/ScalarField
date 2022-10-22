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
	virtual TObjectPtr<USkillUserState> OnLMBPress(TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::OnLMBPress, return nullptr;);
	virtual TObjectPtr<USkillUserState> OnLMBRelease(TObjectPtr<APlayerController> controller) PURE_VIRTUAL(USkillUserState::OnLMBRelease, return nullptr;);
	virtual TObjectPtr<USkillUserState> OnKey1Press(TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::OnKey1Press, return nullptr;);

	virtual void Tick(float deltaTime, TObjectPtr<APlayerController> controller) PURE_VIRTUAL(USkillUserState::Tick, return;);
	virtual void OnEnter(TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::OnEnter, return;);
	virtual void OnLeave(TObjectPtr<AController> controller) PURE_VIRTUAL(USkillUserState::OnLeave, return;);
	
protected:
	TObjectPtr<USkillUserState> _keepCurrentState() { 
		// The skill user must always be in a valid state. Returning nullptr does not invalid the state, but
		// rather signals that the user must not change its state.
		return nullptr;  
	}
};
