// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMState.h"
#include "MovementCommandSetter.h"

#include "NewIdleState.generated.h"

/**
 * \brief State representing a skill user that is not executing any skill.
 */
UCLASS()
class FSM_API UNewIdleState : public UObject, public IFSMState {
    GENERATED_BODY()

public:
    void SetPawn(TObjectPtr<APawn> subjectPawn) override;

    void OnEnter() override;
    void OnLeave() override;

    TScriptInterface<IFSMState> TryExecuteSkillAtIndex(uint32 index) override;

private:
    TWeakObjectPtr<APawn> _subjectPawn = nullptr;
    TWeakObjectPtr<UNewSkillsContainerComponent> _subjectSkillsContainerC = nullptr;
    TWeakInterfacePtr<IMovementCommandSetter> _movementCommandSetter = nullptr;
};
