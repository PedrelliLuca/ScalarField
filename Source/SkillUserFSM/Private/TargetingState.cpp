// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingState.h"

#include "CastingState.h"
#include "IdleState.h"
#include "ManaComponent.h"
#include "MovementCommandSetter.h"
#include "SkillsContainerComponent.h"

TObjectPtr<USkillUserState> UTargetingState::OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) {
	const auto skill = GetSkillInExecution();

	if (!skill->IsValidTarget(_targetBeingSearched, target)) {
		UE_LOG(LogTemp, Warning, TEXT("Invalid target!"));
		return _keepCurrentState();
	}

	FVector casterLocation = controller->GetPawn()->GetActorLocation();
	FVector targetLocation = target->GetActorLocation();
	if (FVector::Distance(casterLocation, targetLocation) > skill->GetMaxDistanceForTarget(_targetBeingSearched)) {
		UE_LOG(LogTemp, Warning, TEXT("Out of range!"));
		return _keepCurrentState();
	}

	skill->SetTarget(_targetBeingSearched, target);
	UE_LOG(LogTemp, Warning, TEXT("Target set!"));

	++_targetBeingSearched;
	if (_targetBeingSearched == skill->NumberOfTargets()) {
		const auto castingState = NewObject<UCastingState>(controller, UCastingState::StaticClass());
		castingState->SetSkillInExecution(skill);
		return castingState;
	}

	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UTargetingState::OnBeginSkillExecution(const int32 skillKey, TObjectPtr<AController> controller) {
	check(skillKey < KEY_ASSIGNABLE_SKILLS);
	const auto pawn = controller->GetPawn();
	const auto skillsContainer = pawn->FindComponentByClass<USkillsContainerComponent>();
	check(IsValid(skillsContainer));

	// keys [1, 2, ..., 9, 0] => index [0, 1, ..., 8, 9]
	const uint32 index = skillKey != 0 ? skillKey - 1 : KEY_ASSIGNABLE_SKILLS - 1;
	const auto skill = skillsContainer->GetSkillAtIndex(index);

	if (!IsValid(skill)) {
		UE_LOG(LogTemp, Warning, TEXT("Skill bound with key &i isn't valid!"), skillKey);
		return _keepCurrentState();
	}

	if (skill == GetSkillInExecution()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is already being casted!"));
		return _keepCurrentState();
	}

	if (skill->IsOnCooldown()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
		return _keepCurrentState();
	}
	
	TObjectPtr<UExecutionState> newState = nullptr;
	if (skill->RequiresTarget()) {
		newState = _abortExecutionForState<UTargetingState>(controller);
	} else {
		newState = _abortExecutionForState<UCastingState>(controller);
	}

	newState->SetSkillInExecution(skill);
	return newState;
}

TObjectPtr<USkillUserState> UTargetingState::OnTick(float deltaTime, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UTargetingState::OnAbort(TObjectPtr<AController> controller) {
	UE_LOG(LogTemp, Error, TEXT("Skill targeting aborted!"));
	return _abortExecutionForState<UIdleState>(controller);
}

void UTargetingState::OnEnter(TObjectPtr<AController> controller) {
	check(GetSkillInExecution()->RequiresTarget());
	UE_LOG(LogTemp, Warning, TEXT("Targeting..."));

	// Movement command update
	const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
	check(movementSetters.Num() == 1);
	const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
	check(movementSetter != nullptr);
	movementSetter->SetMovementMode(GetSkillInExecution()->GetTargetingMovementMode());
}

void UTargetingState::OnLeave(TObjectPtr<AController> controller) {
}
