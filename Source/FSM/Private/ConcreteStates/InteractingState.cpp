// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractingState.h"

#include "InventoryOpenState.h"
#include "SkillExecutionState.h"

void UInteractingState::SetPawn(TObjectPtr<APawn> subjectPawn) {
    // Make sure that the pawn is not valid before setting it.
    if (ensureMsgf(!_subjectPawn.IsValid(), TEXT("State Subject can be set only once and has already been set."))) {
        check(IsValid(subjectPawn));
        _subjectPawn = MoveTemp(subjectPawn);

        _subjectSkillsContainerC = _subjectPawn->FindComponentByClass<UNewSkillsContainerComponent>();
        check(_subjectSkillsContainerC.IsValid());

        // TODO: remove GetController() when the command setter will be on the pawn (required for group control)
        _movementCommandSetter = Cast<IMovementCommandSetter>(_subjectPawn->GetController()->FindComponentByInterface(UMovementCommandSetter::StaticClass()));
        check(_movementCommandSetter.IsValid());

        // TODO: remove GetController() when the interactor will be on the pawn (required for group control)
        _interactor = Cast<IInteractor>(_subjectPawn->GetController()->FindComponentByInterface(UInteractor::StaticClass()));
        check(_interactor.IsValid());
    }
}

void UInteractingState::OnEnter() {
    _movementCommandSetter->StopMovement();
}

void UInteractingState::OnLeave() {
    _interactor->EndInteraction();
}

TScriptInterface<IFSMState> UInteractingState::Tick(const float deltaTime) {
    _interactor->PerformFocusCheck();

    if (_interactor->IsInteracting()) {
        return _keepCurrentState();
    }

    // The interaction is over, back to execution state
    TScriptInterface<IFSMState> executionState = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
    executionState->SetPawn(_subjectPawn.Get());
    return MoveTemp(executionState);
}

TScriptInterface<IFSMState> UInteractingState::TrySetMovementDestination(const FVector& movementDestination) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TryStopMovement() {
    return _keepCurrentState();
}

FStateResponse_TryCastSkill UInteractingState::TryCastSkillAtIndex(const int32 index) {
    auto skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);

    if (skillCastResult.IsFailure()) {
        /* It's impossible we're already executing a skill if we're in this state, something's wrong. */
        check(skillCastResult.GetCastResult() != ESkillCastResult::Fail_InExecution);
        return FStateResponse_TryCastSkill{_keepCurrentState(), MoveTemp(skillCastResult)};
    }

    // TODO?: this causes transition to execution state even if targeting is required. Maybe we want to give the chance to target while keeping the interaction?
    TScriptInterface<IFSMState> executionState = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
    executionState->SetPawn(_subjectPawn.Get());
    return FStateResponse_TryCastSkill{MoveTemp(executionState), MoveTemp(skillCastResult)};
}

TScriptInterface<IFSMState> UInteractingState::TryAbort() {
    TScriptInterface<IFSMState> executionState = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
    executionState->SetPawn(_subjectPawn.Get());
    return MoveTemp(executionState);
}

FStateResponse_TrySetSkillTarget UInteractingState::TrySetSkillTarget(const FSkillTargetPacket& targetPacket) {
    return FStateResponse_TrySetSkillTarget{_keepCurrentState(), TOptional<TVariant<FSkillTargetingResult, FSkillCastResult>>{}};
}

TScriptInterface<IFSMState> UInteractingState::TryInteracting() {
    // This fails if either the interactable is invalid or valid but we're already interacting with it. In both cases, we don't want to interrupt the
    // interaction in progress, so do nothing in case of failure.
    _interactor->PerformInteractionCheck();

    /* We leave this state if the interaction check succeeded but the interactor is already non interacting, meaning that the interaction was instantaneous and
     *there is no need to move to the interaction state. */
    if (!_interactor->IsInteracting()) {
        TScriptInterface<IFSMState> executionState = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
        executionState->SetPawn(_subjectPawn.Get());
        return MoveTemp(executionState);
    }

    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TryToggleInventory() {
    TScriptInterface<IFSMState> inventoryOpenState = NewObject<UInventoryOpenState>(this, UInventoryOpenState::StaticClass());
    inventoryOpenState->SetPawn(_subjectPawn.Get());
    return MoveTemp(inventoryOpenState);
}
