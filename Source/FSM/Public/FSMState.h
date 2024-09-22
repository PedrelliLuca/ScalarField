// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTarget.h"
#include "StateResponses.h"

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

    virtual TScriptInterface<IFSMState> Tick(float deltaTime) = 0;

    virtual TScriptInterface<IFSMState> TrySetMovementDestination(FVector const& movementDestination) = 0;
    virtual TScriptInterface<IFSMState> TryStopMovement() = 0;

    virtual FStateResponse_TryCastSkill TryCastSkillAtIndex(int32 index) = 0;
    virtual TScriptInterface<IFSMState> TryAbort() = 0;
    virtual FStateResponse_TrySetSkillTarget TrySetSkillTarget(FSkillTargetPacket const& targetPacket) = 0;

    virtual TScriptInterface<IFSMState> TryInteracting() = 0;
    virtual TScriptInterface<IFSMState> TryToggleInventory() = 0;

protected:
    TScriptInterface<IFSMState> _keepCurrentState() const { return nullptr; }
};
