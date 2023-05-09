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
    const auto skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);
    return _keepCurrentState();
}
