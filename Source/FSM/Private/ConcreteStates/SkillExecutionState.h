// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMState.h"
#include "NewSkillsContainerComponent.h"

#include "SkillExecutionState.generated.h"

/**
 * \brief State representing a skill user that is not executing any skill.
 */
UCLASS()
class FSM_API USkillExecutionState : public UObject, public IFSMState {
    GENERATED_BODY()

public:
    void SetPawn(TObjectPtr<APawn> subjectPawn) override;

    void OnEnter() override;
    void OnLeave() override;

    TScriptInterface<IFSMState> TryCastSkillAtIndex(int32 index) override;
    TScriptInterface<IFSMState> TryAbortSkillInExecution() override;

private:
    TWeakObjectPtr<APawn> _subjectPawn = nullptr;
    TWeakObjectPtr<UNewSkillsContainerComponent> _subjectSkillsContainerC = nullptr;
};
