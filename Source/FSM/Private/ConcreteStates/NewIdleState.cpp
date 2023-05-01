// Fill out your copyright notice in the Description page of Project Settings.

#include "NewIdleState.h"

#include "MovementCommandSetter.h"
#include "NewSkillsContainerComponent.h"

void UNewIdleState::SetPawn(TObjectPtr<APawn> subjectPawn) {
    // Make sure that the pawn is not valid before setting it.
    if (ensureMsgf(!_subjectPawn.IsValid(), TEXT("State Subject can be set only once and has already been set."))) {
        check(IsValid(subjectPawn));
        _subjectPawn = MoveTemp(subjectPawn);

        _subjectSkillsContainerC = _subjectPawn->FindComponentByClass<UNewSkillsContainerComponent>();
        check(_subjectSkillsContainerC.IsValid());

        const auto controller = _subjectPawn->GetController();
        check(IsValid(controller));
        _movementCommandSetter = Cast<IMovementCommandSetter>(controller->FindComponentByInterface<UMovementCommandSetter>());
        check(_movementCommandSetter.IsValid());
    }
}

void UNewIdleState::OnEnter() {
    _movementCommandSetter->SetDefaultMovementMode();
}

void UNewIdleState::OnLeave() {
}

TScriptInterface<IFSMState> UNewIdleState::TryExecuteSkillAtIndex(const uint32 index) {
    _subjectSkillsContainerC->TryCastSkillAtIndex(index);

    // TODO: return something useful
    return nullptr;
}
