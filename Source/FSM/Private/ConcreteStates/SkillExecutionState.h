// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FSMState.h"
#include "InteractorInterface.h"
#include "MovementCommandSetter.h"
#include "SkillsContainerComponent.h"

#include "SkillExecutionState.generated.h"

/**
 * \brief In this state we are controlling a pawn that can execute skills.
 */
UCLASS()
class FSM_API USkillExecutionState : public UObject, public IFSMState {
    GENERATED_BODY()

public:
    void SetPawn(TObjectPtr<APawn> subjectPawn) override;

    void OnEnter() override;
    void OnLeave() override;

    TScriptInterface<IFSMState> Tick(float deltaTime) override;

    TScriptInterface<IFSMState> TrySetMovementDestination(FVector const& movementDestination) override;
    TScriptInterface<IFSMState> TryStopMovement() override;

    FStateResponse_TryCastSkill TryCastSkillAtIndex(int32 index) override;
    TScriptInterface<IFSMState> TryAbort() override;
    FStateResponse_TrySetSkillTarget TrySetSkillTarget(FSkillTargetPacket const& targetPacket) override;

    TScriptInterface<IFSMState> TryInteracting() override;
    TScriptInterface<IFSMState> TryToggleInventory() override;

private:
    TWeakObjectPtr<APawn> _subjectPawn = nullptr;
    TWeakObjectPtr<USkillsContainerComponent> _subjectSkillsContainerC = nullptr;
    TWeakInterfacePtr<IMovementCommandSetter> _movementCommandSetter = nullptr;
    TWeakInterfacePtr<IInteractor> _interactor = nullptr;
};
