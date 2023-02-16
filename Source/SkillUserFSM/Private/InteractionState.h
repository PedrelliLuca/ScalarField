// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillUserState.h"

#include "InteractionState.generated.h"


/**
 * \brief State representing a skill user that is not executing any skill.
 */
UCLASS(Blueprintable)
class SKILLUSERFSM_API UInteractionState : public USkillUserState {
	GENERATED_BODY()

public:
	TObjectPtr<USkillUserState> OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnInteraction(TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnToggleInventory(TObjectPtr<AController> controller) override { return _keepCurrentState(); }
	TObjectPtr<USkillUserState> OnBeginSkillExecution(TObjectPtr<UAbstractSkill> skill, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnTick(float deltaTime, TObjectPtr<AController> controller) override;
	TObjectPtr<USkillUserState> OnAbort(TObjectPtr<AController> controller) override;

	void OnEnter(TObjectPtr<AController> controller) override;
	void OnLeave(TObjectPtr<AController> controller) override;

	bool IsTickAffectedByPause() const override { return false; }
};
