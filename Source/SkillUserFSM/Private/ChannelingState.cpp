// Fill out your copyright notice in the Description page of Project Settings.


#include "ChannelingState.h"

#include "CastingState.h"
#include "IdleState.h"
#include "ManaComponent.h"
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

	// The owner isn't forced to have a mana component. If it doesn't have one, it means that it can cast its skills for free.
	// Elements in the environment, like turrets that spit fire or clouds that spawn lightning bolts, are examples of this.
	if (const auto manaC = pawn->FindComponentByClass<UManaComponent>()) {
		const double charMana = manaC->GetMana();
		const double manaCost = skill->GetCastManaCost();
		if (charMana < manaCost) {
			UE_LOG(LogTemp, Error, TEXT("Not enough mana to cast skill at index %i"), index);
			return _keepCurrentState();
		}
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
	_elapsedChannelingTime += deltaTime;

	// No mana component? Channeling is free for _caster!
	if (const auto manaC = _caster->FindComponentByClass<UManaComponent>()) {
		const double charMana = manaC->GetMana();

		const double manaCost = skill->GetChannelingManaCost();
		const double manaCostThisFrame = (deltaTime / channelingDuration) * manaCost;

		if (charMana < manaCostThisFrame) {
			UE_LOG(LogTemp, Error, TEXT("Not enough mana to cast skill"));
			return _leaveChannelingForState<UIdleState>(controller);
		}

		manaC->SetMana(charMana - manaCostThisFrame);
	}

	// Execution of skill's channel logic
	skill->ExecuteChannelingTick(deltaTime, _caster.Get());

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

	if (DisablesMovement()) {
		controller->StopMovement();
	}

	_caster = controller->GetPawn();
}

void UChannelingState::OnLeave(const TObjectPtr<AController> controller) {
}
