// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionState.h"

#include "CastingState.h"
#include "IdleState.h"
#include "InteractorInterface.h"
#include "MovementCommandSetter.h"
#include "SkillsContainerComponent.h"
#include "TargetingState.h"

TObjectPtr<USkillUserState> UInteractionState::OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) {
    return _keepCurrentState();
}

TObjectPtr<USkillUserState> UInteractionState::OnInteraction(TObjectPtr<AController> controller) {
    const auto interactors = controller->GetComponentsByInterface(UInteractor::StaticClass());
    // How did you even get in this state if the controller isn't an interactor??
    check(interactors.Num() == 1);
    const auto interactor = Cast<IInteractor>(interactors[0]);
    interactor->PerformInteractionCheck();
    return _keepCurrentState();
}

TObjectPtr<USkillUserState> UInteractionState::OnBeginSkillExecution(TObjectPtr<UAbstractSkill> skill, TObjectPtr<AController> controller) {
    const auto interactors = controller->GetComponentsByInterface(UInteractor::StaticClass());
    // How did you even get in this state if the controller isn't an interactor??
    check(interactors.Num() == 1);
    const auto interactor = Cast<IInteractor>(interactors[0]);
    // If the player begins a skill cast, the pending interaction has to end.
    interactor->EndInteraction();

    if (skill->IsOnCooldown()) {
        UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
        return _keepCurrentState();
    }

    TObjectPtr<UExecutionState> newState = nullptr;
    if (skill->RequiresTarget()) {
        newState = NewObject<UTargetingState>(controller, UTargetingState::StaticClass());
    } else {
        newState = NewObject<UCastingState>(controller, UCastingState::StaticClass());
    }

    newState->SetSkillInExecution(skill);
    return newState;
}

TObjectPtr<USkillUserState> UInteractionState::OnTick(float deltaTime, TObjectPtr<AController> controller) {
    const auto interactors = controller->GetComponentsByInterface(UInteractor::StaticClass());
    // How did you even get in this state if the controller isn't an interactor??
    check(interactors.Num() == 1);
    const auto interactor = Cast<IInteractor>(interactors[0]);

    interactor->PerformFocusCheck();

    // If the interactor is still interaction, it means the interaction isn't over
    if (interactor->IsInteracting()) {
        return _keepCurrentState();
    }

    // The interaction is over, back to idle state
    return NewObject<UIdleState>(controller, UIdleState::StaticClass());
}

TObjectPtr<USkillUserState> UInteractionState::OnAbort(TObjectPtr<AController> controller) {
    const auto interactors = controller->GetComponentsByInterface(UInteractor::StaticClass());
    // How did you even get in this state if the controller isn't an interactor??
    check(interactors.Num() == 1);
    const auto interactor = Cast<IInteractor>(interactors[0]);
    // If the player begins a skill cast, the pending interaction has to end.
    interactor->EndInteraction();

    // The interaction has been aborted, back to idle state
    return NewObject<UIdleState>(controller, UIdleState::StaticClass());
}

void UInteractionState::OnEnter(TObjectPtr<AController> controller) {
    const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
    check(movementSetters.Num() == 1);
    const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
    check(movementSetter != nullptr);

    // Pawns cannot move while interacting
    movementSetter->SetMovementMode(EMovementCommandMode::MCM_Still);
}

void UInteractionState::OnLeave(TObjectPtr<AController> controller) {
#if DO_CHECK
    const auto interactors = controller->GetComponentsByInterface(UInteractor::StaticClass());
    // How did you even get in this state if the controller isn't an interactor??
    check(interactors.Num() == 1);
    const auto interactor = Cast<IInteractor>(interactors[0]);
    check(!interactor->IsInteracting());
#endif
}
