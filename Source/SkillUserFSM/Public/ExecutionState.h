// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractSkill.h"
#include "CoreMinimal.h"
#include "SkillUserState.h"

#include "ExecutionState.generated.h"


/**
 * \brief Abstract class implemented by all states that need an associated skill to perform their operations.
 */
UCLASS(NotBlueprintable, Abstract)
class SKILLUSERFSM_API UExecutionState : public USkillUserState {
	GENERATED_BODY()

public:
	void SetSkillInExecution(TWeakObjectPtr<UAbstractSkill> skillInExecution) { _skillInExecution = skillInExecution; }
	TObjectPtr<UAbstractSkill> GetSkillInExecution() const { 
		check(_skillInExecution.IsValid());
		return _skillInExecution.Get(); 
	}

private:
	TWeakObjectPtr<UAbstractSkill> _skillInExecution = nullptr;
};
