// Fill out your copyright notice in the Description page of Project Settings.

#include "NewStateComponent.h"

#include "ConcreteStates/SkillExecutionState.h"

UNewStateComponent::UNewStateComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UNewStateComponent::TickComponent(float const deltaTime, ELevelTick const tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    auto newState = _state->Tick(deltaTime);
    _performStateTransition(MoveTemp(newState));
}

void UNewStateComponent::TrySetMovementDestination(FVector const& movementDestination) {
    auto newState = _state->TrySetMovementDestination(movementDestination);
    _performStateTransition(MoveTemp(newState));
}

void UNewStateComponent::TryStopMovement() {
    auto newState = _state->TryStopMovement();
    _performStateTransition(MoveTemp(newState));
}

TOptional<FSkillCastResult> UNewStateComponent::TryCastSkillAtIndex(uint32 const index) {
    auto tryCastSkillResponse = _state->TryCastSkillAtIndex(index);
    _performStateTransition(MoveTemp(tryCastSkillResponse.StateResponse));

    return MoveTemp(tryCastSkillResponse.CastResponse);
}

void UNewStateComponent::TryAbort() {
    auto newState = _state->TryAbort();
    _performStateTransition(MoveTemp(newState));
}

TOptional<TVariant<FSkillTargetingResult, FSkillCastResult>> UNewStateComponent::TrySetSkillTarget(FSkillTargetPacket const& targetPacket) {
    auto trySetSkillTargetResponse = _state->TrySetSkillTarget(targetPacket);
    _performStateTransition(MoveTemp(trySetSkillTargetResponse.StateResponse));

    return MoveTemp(trySetSkillTargetResponse.TargetingResponse);
}

void UNewStateComponent::TryInteracting() {
    auto newState = _state->TryInteracting();
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
