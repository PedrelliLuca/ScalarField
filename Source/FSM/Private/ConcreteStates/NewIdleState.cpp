// Fill out your copyright notice in the Description page of Project Settings.

#include "NewIdleState.h"

#include "NewSkillsContainerComponent.h"

void UNewIdleState::SetPawn(TObjectPtr<APawn> subjectPawn) {
    // Make sure that the pawn is not valid before setting it.
    if (ensureMsgf(!_subjectPawn.IsValid(), TEXT("State Subject can be set only once and has already been set."))) {
        check(IsValid(subjectPawn));
        _subjectPawn = MoveTemp(subjectPawn);

        _subjectSkillsContainerC = _subjectPawn->FindComponentByClass<UNewSkillsContainerComponent>();
        check(_subjectSkillsContainerC.IsValid());
    }
}

void UNewIdleState::OnEnter() {
}

void UNewIdleState::OnLeave() {
}

TScriptInterface<IFSMState> UNewIdleState::TryCastSkillAtIndex(const int32 index) {
    const auto skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);
    if (skillCastResult.IsFailure()) {
        // TODO: properly manage error text via error system
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillCastResult.GetErrorText().ToString());
    }

    return _keepCurrentState();
}
