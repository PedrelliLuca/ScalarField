// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillExecutionState.h"

#include "InteractingState.h"
#include "InventoryOpenState.h"

void USkillExecutionState::SetPawn(TObjectPtr<APawn> subjectPawn) {
    // Make sure that the pawn is not valid before setting it.
    if (ensureMsgf(!_subjectPawn.IsValid(), TEXT("State Subject can be set only once and has already been set."))) {
        check(IsValid(subjectPawn));
        _subjectPawn = MoveTemp(subjectPawn);

        _subjectSkillsContainerC = _subjectPawn->FindComponentByClass<USkillsContainerComponent>();
        check(_subjectSkillsContainerC.IsValid());

        // TODO: remove GetController() when the command setter will be on the pawn (required for group control)
        _movementCommandSetter = Cast<IMovementCommandSetter>(_subjectPawn->GetController()->FindComponentByInterface(UMovementCommandSetter::StaticClass()));
        check(_movementCommandSetter.IsValid());

        // TODO: remove GetController() when the interactor will be on the pawn (required for group control)
        // This component is not mandatory, AI might not have it
        _interactor = Cast<IInteractor>(_subjectPawn->GetController()->FindComponentByInterface(UInteractor::StaticClass()));
    }
}

void USkillExecutionState::OnEnter() {
}

void USkillExecutionState::OnLeave() {
    _subjectSkillsContainerC->AbortWaitingSkill();
}

TScriptInterface<IFSMState> USkillExecutionState::Tick(const float deltaTime) {
    _movementCommandSetter->MovementTick(deltaTime);
    if (_interactor.IsValid()) {
        _interactor->PerformFocusCheck();
    }
    return _keepCurrentState();
}

TScriptInterface<IFSMState> USkillExecutionState::TrySetMovementDestination(const FVector& movementDestination) {
    _movementCommandSetter->SetDestination(movementDestination);
    return _keepCurrentState();
}

TScriptInterface<IFSMState> USkillExecutionState::TryStopMovement() {
    _movementCommandSetter->StopMovement();
    return _keepCurrentState();
}

FStateResponse_TryCastSkill USkillExecutionState::TryCastSkillAtIndex(const int32 index) {
    auto skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);

    /* In case the skill we're trying to cast is in execution, we abort it and try casting it again. There is no guarantee that the 2nd time we'll succeed BUT,
     * since we aborted, this time we must not get ESkillCastResult::Fail_InExecution. */
    if (skillCastResult.GetCastResult() == ESkillCastResult::Fail_InExecution) {
        [[maybe_unused]] const auto abortSuccessful = _subjectSkillsContainerC->AbortSkillInExecution();
        check(abortSuccessful);

        skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);
        check(skillCastResult.GetCastResult() != ESkillCastResult::Fail_InExecution);
    }

    return FStateResponse_TryCastSkill{_keepCurrentState(), MoveTemp(skillCastResult)};
}

TScriptInterface<IFSMState> USkillExecutionState::TryAbort() {
    _subjectSkillsContainerC->AbortSkillInExecution();
    return _keepCurrentState();
}

FStateResponse_TrySetSkillTarget USkillExecutionState::TrySetSkillTarget(const FSkillTargetPacket& targetPacket) {
    auto skillTargetingResult = _subjectSkillsContainerC->TryAddTargetToWaitingSkill(targetPacket);
    TOptional<TVariant<FSkillTargetingResult, FSkillCastResult>> optionalResultVariant{};

    if (skillTargetingResult.IsSet()) {
        optionalResultVariant = TVariant<FSkillTargetingResult, FSkillCastResult>{TInPlaceType<FSkillTargetingResult>{}, *skillTargetingResult};

        if (skillTargetingResult.GetValue().GetTargetingResult() == ESkillTargetingResult::Success_IntoCast) {
            auto optionalSkillCastResult = _subjectSkillsContainerC->TryCastWaitingSkill();
            check(optionalSkillCastResult.IsSet());
            auto skillCastResult = optionalSkillCastResult.GetValue();

            /* In case the skill we're trying to cast is in execution, we abort it and try casting it again. There is no guarantee that the 2nd time we'll
             * succeed BUT, since we aborted, this time we must not get ESkillCastResult::Fail_InExecution. */
            if (skillCastResult.GetCastResult() == ESkillCastResult::Fail_InExecution) {
                [[maybe_unused]] const auto abortSuccessful = _subjectSkillsContainerC->AbortSkillInExecution();
                check(abortSuccessful);

                optionalSkillCastResult = _subjectSkillsContainerC->TryCastWaitingSkill();
                check(optionalSkillCastResult.IsSet());
                check(skillCastResult.GetCastResult() != ESkillCastResult::Fail_InExecution);
            }

            optionalResultVariant = TVariant<FSkillTargetingResult, FSkillCastResult>{TInPlaceType<FSkillCastResult>{}, MoveTemp(skillCastResult)};
        }
    }

    return FStateResponse_TrySetSkillTarget{_keepCurrentState(), MoveTemp(optionalResultVariant)};
}

TScriptInterface<IFSMState> USkillExecutionState::TryInteracting() {
    // Not all pawns might have an _interactor. However, if they don't, they shouldn't be calling this function!!
    check(_interactor.IsValid());

    const bool interactionSuccessful = _interactor->PerformInteractionCheck();
    if (interactionSuccessful) {
        _subjectSkillsContainerC->AbortSkillInExecution();
    }

    /* We keep this state if one of the following is true:
     * 1. The interaction check failed
     * 2. The interaction check succeeded but the interactor is already non interacting, meaning that the interaction
     *	  was instantaneous and there is no need to move to the interaction state.
     */
    if (!interactionSuccessful || !_interactor->IsInteracting()) {
        return _keepCurrentState();
    }

    TScriptInterface<IFSMState> interactingState = NewObject<UInteractingState>(this, UInteractingState::StaticClass());
    interactingState->SetPawn(_subjectPawn.Get());
    return MoveTemp(interactingState);
}

TScriptInterface<IFSMState> USkillExecutionState::TryToggleInventory() {
    TScriptInterface<IFSMState> inventoryOpenState = NewObject<UInventoryOpenState>(this, UInventoryOpenState::StaticClass());
    inventoryOpenState->SetPawn(_subjectPawn.Get());
    return MoveTemp(inventoryOpenState);
}
