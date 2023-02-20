// Fill out your copyright notice in the Description page of Project Settings.


#include "StateComponent.h"

#include "IdleState.h"
#include "InventoryLookupState.h"
#include "SkillUserState.h"

bool UStateComponent::IsCurrentStateAffectedByPause() const {
	return _state->IsTickAffectedByPause();
}

bool UStateComponent::IsLookingAtWidget() const {
	return _state->IsA<UInventoryLookupState>();
}

void UStateComponent::PerformTargetingBehavior(TObjectPtr<AActor> target) {
	auto newState = _state->OnTargeting(MoveTemp(target), _ownerController.Get());
	_performStateTransitionRoutine(MoveTemp(newState));
}

bool UStateComponent::PerformSkillExecutionBehavior(TObjectPtr<UAbstractSkill> skill) {
	auto newState = _state->OnBeginSkillExecution(MoveTemp(skill), _ownerController.Get());
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
		_state = newState;
		_state->OnEnter(_ownerController.Get());
		return true;
	}

	return false;
}
