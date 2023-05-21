// Fill out your copyright notice in the Description page of Project Settings.

#include "NewStateComponent.h"

#include "ConcreteStates/SkillExecutionState.h"

void UNewStateComponent::TryCastSkillAtIndex(const uint32 index) {
    auto newState = _state->TryCastSkillAtIndex(index);
    _performStateTransition(MoveTemp(newState));
}

void UNewStateComponent::TryAbortSkillInExecution() {
    auto newState = _state->TryAbortSkillInExecution();
    _performStateTransition(MoveTemp(newState));
}

void UNewStateComponent::TrySetSkillTarget(const FSkillTargetPacket& targetPacket) {
    auto newState = _state->TrySetSkillTarget(targetPacket);
    _performStateTransition(MoveTemp(newState));
}

void UNewStateComponent::TryToggleInventory() {
    auto newState = _state->TryToggleInventory();
    _performStateTransition(MoveTemp(newState));
}

void UNewStateComponent::BeginPlay() {
    Super::BeginPlay();

    _ownerPawn = Cast<APawn>(GetOwner());
    check(_ownerPawn.IsValid());

    _state = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
    _state->SetPawn(_ownerPawn.Get());
    _state->OnEnter();
}

void UNewStateComponent::_performStateTransition(TScriptInterface<IFSMState>&& newState) {
    if (IsValid(newState.GetObject())) {
        _state->OnLeave();
        _state = MoveTemp(newState);
        _state->OnEnter();
    }
}
