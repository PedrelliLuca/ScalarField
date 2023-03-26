// Fill out your copyright notice in the Description page of Project Settings.

#include "IdleState.h"

#include "CastingState.h"
#include "InteractionState.h"
#include "InteractorInterface.h"
#include "InventoryLookupState.h"
#include "MovementCommandSetter.h"
#include "TargetingState.h"

TObjectPtr<USkillUserState> UIdleState::OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) {
    return _keepCurrentState();
}

TObjectPtr<USkillUserState> UIdleState::OnInteraction(TObjectPtr<AController> controller) {
    const auto interactors = controller->GetComponentsByInterface(UInteractor::StaticClass());
    // How did you even get in this state if the controller isn't an interactor??
    check(interactors.Num() <= 1);
    if (interactors.IsEmpty()) {
        return _keepCurrentState();
    }

    const auto interactor = Cast<IInteractor>(interactors[0]);
    const bool interactionSuccessful = interactor->PerformInteractionCheck();

    /* We stay in idle if one of the following is true:
     * 1. The interaction check failed
     * 2. The interaction check succeeded but the interactor is already non interacting, meaning that the interaction
     *	  was instantaneous and there is no need to move to the interaction state.
     */
    if (!interactionSuccessful || !interactor->IsInteracting()) {
        return _keepCurrentState();
    }

    return NewObject<UInteractionState>(controller, UInteractionState::StaticClass());
}

TObjectPtr<USkillUserState> UIdleState::OnToggleInventory(TObjectPtr<AController> controller) {
    return NewObject<UInventoryLookupState>(controller, UInventoryLookupState::StaticClass());
}

TObjectPtr<USkillUserState> UIdleState::OnBeginSkillExecution(TObjectPtr<UAbstractSkill> skill, TObjectPtr<AController> controller) {
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

TObjectPtr<USkillUserState> UIdleState::OnTick(float deltaTime, TObjectPtr<AController> controller) {
    const auto interactors = controller->GetComponentsByInterface(UInteractor::StaticClass());
    // How did you even get in this state if the controller isn't an interactor??
    check(interactors.Num() <= 1);
    if (!interactors.IsEmpty()) {
        const auto interactor = Cast<IInteractor>(interactors[0]);
        interactor->PerformFocusCheck();
    }
    return _keepCurrentState();
}

TObjectPtr<USkillUserState> UIdleState::OnAbort(TObjectPtr<AController> controller) {
    return _keepCurrentState();
}

void UIdleState::OnEnter(TObjectPtr<AController> controller) {
    // Movement command update
    const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
    check(movementSetters.Num() == 1);
    const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
    check(movementSetter != nullptr);
    movementSetter->SetDefaultMovementMode();
}

void UIdleState::OnLeave(TObjectPtr<AController> controller) {
}
