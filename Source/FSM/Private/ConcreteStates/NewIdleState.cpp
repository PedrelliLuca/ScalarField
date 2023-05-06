// Fill out your copyright notice in the Description page of Project Settings.

#include "NewIdleState.h"

#include "NewCastState.h"
#include "NewChannelingState.h"
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
}

void UNewIdleState::OnLeave() {
}

TScriptInterface<IFSMState> UNewIdleState::TryCastSkillAtIndex(const int32 index) {
    const auto skillCastResult = _subjectSkillsContainerC->TryCastSkillAtIndex(index);

    TScriptInterface<IFSMState> newState = _keepCurrentState();
    
    // TODO: Manage castResultValue == ESkillCastResult::Fail_MissingTarget
    const auto castResultValue = skillCastResult.GetCastResult();
    if (castResultValue == ESkillCastResult::Deferred) {
        newState = NewObject<UNewCastState>(GetOuter(), UNewCastState::StaticClass());
        newState->SetPawn(_subjectPawn.Get());
        // TODO: tell cast state to bind to skills container's delegate
    } else if (castResultValue == ESkillCastResult::Success_IntoChanneling) {
        newState = NewObject<UNewChannelingState>(GetOuter(), UNewChannelingState::StaticClass());
        newState->SetPawn(_subjectPawn.Get());
        // TODO: tell channeling state to bind to skills container's delegate
    } else if (skillCastResult.IsFailure()) {
        // TODO: send error to manager of some kind
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillCastResult.GetErrorText().ToString());
    }

    return newState;
}
