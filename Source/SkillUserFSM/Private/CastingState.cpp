// Fill out your copyright notice in the Description page of Project Settings.


#include "CastingState.h"

#include "ChannelingState.h"
#include "IdleState.h"
#include "MovementCommandSetter.h"
#include "SkillsContainerComponent.h"
#include "TargetingState.h"


TObjectPtr<USkillUserState> UCastingState::OnTargeting(const TObjectPtr<AActor> target, const TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UCastingState::OnBeginSkillExecution(const int32 skillKey, const TObjectPtr<AController> controller) {
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
		newState = NewObject<UTargetingState>(controller, UTargetingState::StaticClass());
	}
	else {
		newState = NewObject<UCastingState>(controller, UCastingState::StaticClass());
	}

	newState->SetSkillInExecution(skill);
	return newState;
}

TObjectPtr<USkillUserState> UCastingState::OnTick(const float deltaTime, const TObjectPtr<AController> controller) {
	const auto skill = GetSkillInExecution();
	const double castDuration = GetSkillInExecution()->GetCastTime();

	if (_elapsedCastTime + deltaTime >= castDuration) {
		if (_casterManaC.IsValid()) { // No mana component == free skill
			const auto currentMana = _casterManaC->GetMana();
			_casterManaC->SetMana(currentMana - _manaLeftToPay);
		}

		skill->ExecuteCast(_caster.Get());
		return _determineStateBasedOnSkillChanneling(skill, controller);
	}

	if (_casterManaC.IsValid()) { // No mana component == free skill
		const auto currentMana = _casterManaC->GetMana();

		const double manaCost = skill->GetCastManaCost();
		const double manaCostThisFrame = (deltaTime / castDuration) * manaCost;
		_casterManaC->SetMana(currentMana - manaCostThisFrame);
		_manaLeftToPay -= manaCostThisFrame;
	}

	_elapsedCastTime += deltaTime;
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UCastingState::OnSkillExecutionAborted(const TObjectPtr<AController> controller) {
	UE_LOG(LogTemp, Error, TEXT("Skill cast aborted!"));

	GetSkillInExecution()->RemoveAllTargets();
	const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
	return idleState;
}

void UCastingState::OnEnter(const TObjectPtr<AController> controller) {
	UE_LOG(LogTemp, Warning, TEXT("Skill cast begun!"));

	// Movement command update
	const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
	check(movementSetters.Num() == 1);
	const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
	check(movementSetter != nullptr);
	movementSetter->SetMovementMode(GetSkillInExecution()->GetCastMovementMode());

	_caster = controller->GetPawn();
	_casterManaC = _caster->FindComponentByClass<UManaComponent>();

	const auto skill = GetSkillInExecution();
	_manaLeftToPay = skill->GetCastManaCost();
}

void UCastingState::OnLeave(const TObjectPtr<AController> controller) {
}

TObjectPtr<USkillUserState> UCastingState::_determineStateBasedOnSkillChanneling(const TObjectPtr<UAbstractSkill> skill, const TObjectPtr<AController> controller)
{
	if (GetSkillInExecution()->RequiresChanneling()) {
		const auto channelingState = NewObject<UChannelingState>(controller, UChannelingState::StaticClass());
		channelingState->SetSkillInExecution(GetSkillInExecution());
		return channelingState;
	}

	const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
	return idleState;
}
