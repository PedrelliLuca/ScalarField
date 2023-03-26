// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
    TObjectPtr<USkillUserState> OnInteraction(TObjectPtr<AController> controller) override { return _keepCurrentState(); }
    TObjectPtr<USkillUserState> OnToggleInventory(TObjectPtr<AController> controller) override { return _keepCurrentState(); }
    bool IsTickAffectedByPause() const override { return true; }

    void SetSkillInExecution(TWeakObjectPtr<UAbstractSkill> skillInExecution) { _skillInExecution = skillInExecution; }
    TObjectPtr<UAbstractSkill> GetSkillInExecution() const {
        check(_skillInExecution.IsValid());
        return _skillInExecution.Get();
    }

protected:
    template <typename S> TObjectPtr<S> _abortExecutionForState(TObjectPtr<AController> controller) {
        static_assert(TIsDerivedFrom<S, USkillUserState>::Value, "S must derived from USkillUserState abstract class");
        static_assert(!TIsSame<S, USkillUserState>::Value, "S must not be of type USkillUserState");
        static_assert(!TIsSame<S, UExecutionState>::Value, "S must not be of type UExecutionState");

        check(_skillInExecution.IsValid());
        _skillInExecution->Abort();
        return NewObject<S>(controller);
    }

private:
    TWeakObjectPtr<UAbstractSkill> _skillInExecution = nullptr;
};
