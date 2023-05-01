// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FSMState.generated.h"

UINTERFACE(MinimalAPI)
class UFSMState : public UInterface {
    GENERATED_BODY()
};

/** \brief Represents the state of a pawn in the ScalarField game. */
class IFSMState {
    GENERATED_BODY()

public:
    virtual void SetPawn(TObjectPtr<APawn> subjectPawn) = 0;

    virtual void OnEnter() = 0;
    virtual void OnLeave() = 0;
    
    virtual TScriptInterface<IFSMState> TryExecuteSkillAtIndex(uint32 index) = 0;
};