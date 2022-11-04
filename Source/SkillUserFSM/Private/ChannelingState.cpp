// Fill out your copyright notice in the Description page of Project Settings.


#include "ChannelingState.h"

#include "CastingState.h"
#include "IdleState.h"
#include "MovementCommandSetter.h"
#include "SkillsContainerComponent.h"
#include "TargetingState.h"

TObjectPtr<USkillUserState> UChannelingState::OnTargeting(TObjectPtr<AActor> target, const TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnBeginSkillExecution(const int32 skillKey, const TObjectPtr<AController> controller) {
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
		newState = _leaveChannelingForState<UTargetingState>(controller);
	}
	else {
		newState = _leaveChannelingForState<UCastingState>(controller);
	}

	newState->SetSkillInExecution(skill);
	return newState;
}

TObjectPtr<USkillUserState> UChannelingState::OnTick(float deltaTime, const TObjectPtr<AController> controller) {
	const auto skill = GetSkillInExecution();
	const double channelingDuration = GetSkillInExecution()->GetChannelingTime();
	if (_elapsedChannelingTime + deltaTime >= channelingDuration) {
		// Apply one last mana payment with reduced deltaTime
		deltaTime = channelingDuration - _elapsedChannelingTime;
	}

	// No mana component == free skill
	if (_casterManaC.IsValid()) {
		const double charMana = _casterManaC->GetMana();

		const double manaCost = skill->GetChannelingManaCost();
		const double manaCostThisFrame = (deltaTime / channelingDuration) * manaCost;

		if (charMana < manaCostThisFrame) {
			UE_LOG(LogTemp, Error, TEXT("Not enough mana to keep channeling skill"));
			return _leaveChannelingForState<UIdleState>(controller);
		}

		_casterManaC->SetMana(charMana - manaCostThisFrame);
	}

	// Execution of skill's channel logic
	skill->ExecuteChannelingTick(deltaTime, _caster.Get());
	_elapsedChannelingTime += deltaTime;

	if (FMath::IsNearlyEqual(_elapsedChannelingTime, channelingDuration)) {
		return _leaveChannelingForState<UIdleState>(controller);
	}

	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnSkillExecutionAborted(const TObjectPtr<AController> controller) {
	UE_LOG(LogTemp, Error, TEXT("Skill channeling aborted!"));
	return _leaveChannelingForState<UIdleState>(controller);
}

void UChannelingState::OnEnter(const TObjectPtr<AController> controller) {
	const auto skill = GetSkillInExecution();
	check(IsValid(skill) && skill->RequiresChanneling() && skill->GetChannelingManaCost() > 0.);
	UE_LOG(LogTemp, Warning, TEXT("Channeling..."));

	// Movement command update
	const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
	check(movementSetters.Num() == 1);
	const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
	check(movementSetter != nullptr);
	movementSetter->SetMovementMode(GetSkillInExecution()->GetChannelingMovementMode());

	_caster = controller->GetPawn();
	_casterManaC = _caster->FindComponentByClass<UManaComponent>();
}

void UChannelingState::OnLeave(const TObjectPtr<AController> controller) {
}
