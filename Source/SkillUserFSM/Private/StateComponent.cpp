// Fill out your copyright notice in the Description page of Project Settings.

#include "StateComponent.h"

#include "ExecutionState.h"
#include "IdleState.h"
#include "InventoryLookupState.h"
#include "SkillUserState.h"

bool UStateComponent::IsCurrentStateAffectedByPause() const {
    return _state->IsTickAffectedByPause();
}

bool UStateComponent::PerformTargetingBehavior(TObjectPtr<AActor> target) {
    auto newState = _state->OnTargeting(MoveTemp(target), _ownerController.Get());

    // If OnTargeting() doesn't produce a new state, it means that the targeting failed.
    return _performStateTransitionRoutine(MoveTemp(newState));
}

bool UStateComponent::PerformSkillExecutionBehavior(TObjectPtr<UAbstractSkill> skill) {
    auto newState = _state->OnBeginSkillExecution(MoveTemp(skill), _ownerController.Get());

    // If OnBeginSkillExecution() doesn't produce a new state, it means that the skill execution cannot start.
    return _performStateTransitionRoutine(MoveTemp(newState));
}

void UStateComponent::PerformInteractionBehavior() {
    auto newState = _state->OnInteraction(_ownerController.Get());
    _performStateTransitionRoutine(MoveTemp(newState));
}

void UStateComponent::PerformInventoryToggleBehavior() {
    auto newState = _state->OnToggleInventory(_ownerController.Get());
    _performStateTransitionRoutine(MoveTemp(newState));
}

void UStateComponent::PerformTickBehavior(const float deltaTime) {
    auto newState = _state->OnTick(deltaTime, _ownerController.Get());
    _performStateTransitionRoutine(MoveTemp(newState));
}

void UStateComponent::PerformAbortBehavior() {
    auto newState = _state->OnAbort(_ownerController.Get());
    _performStateTransitionRoutine(MoveTemp(newState));
}

void UStateComponent::BeginPlay() {
    Super::BeginPlay();

    // This component can be assigned only to AControllers
    const TWeakObjectPtr<AController> ownerController = Cast<AController>(GetOwner());
    check(ownerController.IsValid());
    _ownerController = ownerController;

    _state = NewObject<UIdleState>(this, UIdleState::StaticClass());
    _state->OnEnter(_ownerController.Get());
}

bool UStateComponent::_performStateTransitionRoutine(const TObjectPtr<USkillUserState> newState) {
    check(_ownerController.IsValid());

    // If the new state isn't valid, that means that we didn't transition and kept the current state
    if (IsValid(newState)) {
        _state->OnLeave(_ownerController.Get());

        // Warn anyone listening if we moved from a Skill Execution state to another or viceversa
        const auto isOldStateExecutionState = _state->GetClass()->IsChildOf(UExecutionState::StaticClass());
        const auto isNewStateExecutionState = newState->GetClass()->IsChildOf(UExecutionState::StaticClass());
        if (isNewStateExecutionState && !isOldStateExecutionState) {
            _onSkillExecutionBegin.Broadcast();
        } else if (!isNewStateExecutionState && isOldStateExecutionState) {
            _onSkillExecutionEnd.Broadcast();
        }

        _state = newState;
        _state->OnEnter(_ownerController.Get());
        return true;
    }

    return false;
}
