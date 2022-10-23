// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillUserState.h"

#include "IdleState.generated.h"


/**
 * \brief State representing a skill user that is not executing any skill.
 */
UCLASS(Blueprintable)
class SKILLUSERFSM_API UIdleState : public USkillUserState {
	GENERATED_BODY()

public:
	TObjectPtr<USkillUserState> OnTargeting(TObjectPtr<APlayerController> controller) override;
	TObjectPtr<USkillUserState> OnBeginSkillExecution(int32 skillKey, TObjectPtr<AController> controller) override;

	TObjectPtr<USkillUserState> OnTick(float deltaTime, TObjectPtr<APlayerController> controller) override;
	void OnEnter(TObjectPtr<AController> controller) override;
	void OnLeave(TObjectPtr<AController> controller) override;

	bool DisablesMovement() const override { return false; }
};
