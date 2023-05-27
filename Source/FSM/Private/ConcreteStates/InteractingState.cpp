// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractingState.h"

void UInteractingState::SetPawn(TObjectPtr<APawn> subjectPawn) {
    // Make sure that the pawn is not valid before setting it.
    if (ensureMsgf(!_subjectPawn.IsValid(), TEXT("State Subject can be set only once and has already been set."))) {
        check(IsValid(subjectPawn));
        _subjectPawn = MoveTemp(subjectPawn);
    }
}

void UInteractingState::OnEnter() {
}

void UInteractingState::OnLeave() {
}

TScriptInterface<IFSMState> UInteractingState::Tick(const float deltaTime) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TrySetMovementDestination(const FVector& movementDestination) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TryStopMovement() {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TryCastSkillAtIndex(int32 index) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TryAbortSkillInExecution() {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TrySetSkillTarget(const FSkillTargetPacket& targetPacket) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TryInteracting() {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TryToggleInventory() {
    return _keepCurrentState();
}
