// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

#include "CastingState.h"
#include "ManaComponent.h"
#include "MovementCommandSetter.h"
#include "SkillsContainerComponent.h"
#include "TargetingState.h"

TObjectPtr<USkillUserState> UIdleState::OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UIdleState::OnBeginSkillExecution(const int32 skillKey, TObjectPtr<AController> controller) {
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
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UIdleState::OnSkillExecutionAborted(TObjectPtr<AController> controller) {
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
