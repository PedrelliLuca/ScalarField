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

        // TODO: edit this when the movement setter will be on the pawn
        _movementCommandSetter = Cast<IMovementCommandSetter>(_subjectPawn->GetController()->FindComponentByInterface(UMovementCommandSetter::StaticClass()));
        check(_movementCommandSetter.IsValid());

        _interactor = Cast<IInteractor>(_subjectPawn->FindComponentByInterface(UInteractor::StaticClass()));
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

TScriptInterface<IFSMState> UInteractingState::TryCastSkillAtIndex(const int32 index) {
    auto skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);

    /* In case the skill we're trying to cast is in execution, we abort it and try casting it again. There is no guarantee that the 2nd time we'll succeed BUT,
     * since we aborted, this time we must not get ESkillCastResult::Fail_InExecution. */
    if (skillCastResult.GetCastResult() == ESkillCastResult::Fail_InExecution) {
        [[maybe_unused]] const auto abortSuccessful = _subjectSkillsContainerC->AbortSkillInExecution();
        check(abortSuccessful);

        skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);
        check(skillCastResult.GetCastResult() != ESkillCastResult::Fail_InExecution);
    }

    TScriptInterface<IFSMState> executionState = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
    executionState->SetPawn(_subjectPawn.Get());
    return MoveTemp(executionState);
}

TScriptInterface<IFSMState> UInteractingState::TryAbortSkillInExecution() {
    TScriptInterface<IFSMState> executionState = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
    executionState->SetPawn(_subjectPawn.Get());
    return MoveTemp(executionState);
}

TScriptInterface<IFSMState> UInteractingState::TrySetSkillTarget(const FSkillTargetPacket& targetPacket) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInteractingState::TryInteracting() {
    const bool interactionSuccessful = _interactor->PerformInteractionCheck();

    /* We leave this state if one of the following is true:
     * 1. The interaction check failed
     * 2. The interaction check succeeded but the interactor is already non interacting, meaning that the interaction
     *	  was instantaneous and there is no need to move to the interaction state.
     */
    if (!interactionSuccessful || !_interactor->IsInteracting()) {
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
