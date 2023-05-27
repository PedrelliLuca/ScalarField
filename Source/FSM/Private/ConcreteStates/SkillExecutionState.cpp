// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillExecutionState.h"

#include "InventoryOpenState.h"

void USkillExecutionState::SetPawn(TObjectPtr<APawn> subjectPawn) {
    // Make sure that the pawn is not valid before setting it.
    if (ensureMsgf(!_subjectPawn.IsValid(), TEXT("State Subject can be set only once and has already been set."))) {
        check(IsValid(subjectPawn));
        _subjectPawn = MoveTemp(subjectPawn);

        _subjectSkillsContainerC = _subjectPawn->FindComponentByClass<UNewSkillsContainerComponent>();
        check(_subjectSkillsContainerC.IsValid());

        // TODO: edit this when the movement setter will be on the pawn
        _movementCommandSetter = Cast<IMovementCommandSetter>(_subjectPawn->GetController()->FindComponentByInterface((UMovementCommandSetter::StaticClass())));
        check(_movementCommandSetter.IsValid());
    }
}

void USkillExecutionState::OnEnter() {
}

void USkillExecutionState::OnLeave() {
    _subjectSkillsContainerC->AbortWaitingSkill();
}

TScriptInterface<IFSMState> USkillExecutionState::Tick(const float deltaTime) {
    _movementCommandSetter->MovementTick(deltaTime);
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

TScriptInterface<IFSMState> USkillExecutionState::TryCastSkillAtIndex(const int32 index) {
    auto skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);

    /* In case the skill we're trying to cast is in execution, we abort it and try casting it again. There is no guarantee that the 2nd time we'll succeed BUT,
     * since we aborted, this time we must not get ESkillCastResult::Fail_InExecution. */
    if (skillCastResult.GetCastResult() == ESkillCastResult::Fail_InExecution) {
        [[maybe_unused]] const auto abortSuccessful = _subjectSkillsContainerC->AbortSkillInExecution();
        check(abortSuccessful);

        skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);
        check(skillCastResult.GetCastResult() != ESkillCastResult::Fail_InExecution);
    }

    return _keepCurrentState();
}

TScriptInterface<IFSMState> USkillExecutionState::TryAbortSkillInExecution() {
    _subjectSkillsContainerC->AbortSkillInExecution();
    return _keepCurrentState();
}

TScriptInterface<IFSMState> USkillExecutionState::TrySetSkillTarget(const FSkillTargetPacket& targetPacket) {
    const auto skillTargetingResult = _subjectSkillsContainerC->TryAddTargetToWaitingSkill(targetPacket);

    if (skillTargetingResult.IsSet() && skillTargetingResult.GetValue().GetTargetingResult() == ESkillTargetingResult::Success_IntoCast) {
        auto optionalSkillCastResult = _subjectSkillsContainerC->TryCastWaitingSkill();
        check(optionalSkillCastResult.IsSet());
        const auto skillCastResult = optionalSkillCastResult.GetValue();

        /* In case the skill we're trying to cast is in execution, we abort it and try casting it again. There is no guarantee that the 2nd time we'll succeed
         * BUT, since we aborted, this time we must not get ESkillCastResult::Fail_InExecution. */
        if (skillCastResult.GetCastResult() == ESkillCastResult::Fail_InExecution) {
            [[maybe_unused]] const auto abortSuccessful = _subjectSkillsContainerC->AbortSkillInExecution();
            check(abortSuccessful);

            optionalSkillCastResult = _subjectSkillsContainerC->TryCastWaitingSkill();
            check(optionalSkillCastResult.IsSet());
            check(skillCastResult.GetCastResult() != ESkillCastResult::Fail_InExecution);
        }
    }

    return _keepCurrentState();
}

TScriptInterface<IFSMState> USkillExecutionState::TryToggleInventory() {
    TScriptInterface<IFSMState> inventoryOpenState = NewObject<UInventoryOpenState>(this, UInventoryOpenState::StaticClass());
    inventoryOpenState->SetPawn(_subjectPawn.Get());
    return MoveTemp(inventoryOpenState);
}
