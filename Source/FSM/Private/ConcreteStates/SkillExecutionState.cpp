// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillExecutionState.h"

#include "NewSkillsContainerComponent.h"

void USkillExecutionState::SetPawn(TObjectPtr<APawn> subjectPawn) {
    // Make sure that the pawn is not valid before setting it.
    if (ensureMsgf(!_subjectPawn.IsValid(), TEXT("State Subject can be set only once and has already been set."))) {
        check(IsValid(subjectPawn));
        _subjectPawn = MoveTemp(subjectPawn);

        _subjectSkillsContainerC = _subjectPawn->FindComponentByClass<UNewSkillsContainerComponent>();
        check(_subjectSkillsContainerC.IsValid());
    }
}

void USkillExecutionState::OnEnter() {
}

void USkillExecutionState::OnLeave() {
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
