// Fill out your copyright notice in the Description page of Project Settings.

#include "NewStateComponent.h"

#include "ConcreteStates/NewIdleState.h"

void UNewStateComponent::TryExecuteSkillAtIndex(uint32 index) {
    auto newState = _state->TryExecuteSkillAtIndex(index);
    _performStateTransition(MoveTemp(newState));
}

void UNewStateComponent::BeginPlay() {
    Super::BeginPlay();

    _ownerPawn = Cast<APawn>(GetOwner());
    check(_ownerPawn.IsValid());

    _state = NewObject<UNewIdleState>(this, UNewIdleState::StaticClass());
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
