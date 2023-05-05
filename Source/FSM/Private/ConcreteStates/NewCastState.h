// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMState.h"

#include "NewCastState.generated.h"

/**
 * \brief State representing a skill user that is casting a skill.
 */
UCLASS()
class FSM_API UNewCastState : public UObject, public IFSMState {
    GENERATED_BODY()

public:
    void SetPawn(TObjectPtr<APawn> subjectPawn) override;

    void OnEnter() override;
    void OnLeave() override;

    TScriptInterface<IFSMState> TryCastSkillAtIndex(int32 index) override;
};
