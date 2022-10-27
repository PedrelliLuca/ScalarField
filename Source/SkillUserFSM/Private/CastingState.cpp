// Fill out your copyright notice in the Description page of Project Settings.


#include "CastingState.h"

#include "ChannelingState.h"
#include "IdleState.h"
#include "ManaComponent.h"
#include "SkillsContainerComponent.h"
#include "TargetingState.h"


TObjectPtr<USkillUserState> UCastingState::OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UCastingState::OnBeginSkillExecution(const int32 skillKey, TObjectPtr<AController> controller) {
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

TObjectPtr<USkillUserState> UCastingState::OnTick(float deltaTime, TObjectPtr<AController> controller) {
	if (!_bEndCasting) {
		return _keepCurrentState();

	}

	if (GetSkillInExecution()->RequiresChanneling()) {
		const auto channelingState = NewObject<UChannelingState>(controller, UChannelingState::StaticClass());
		channelingState->SetSkillInExecution(GetSkillInExecution());
		return channelingState;
	}

	const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
	return idleState;
}

TObjectPtr<USkillUserState> UCastingState::OnSkillExecutionAborted(TObjectPtr<AController> controller) {
	// TODO?: maybe in the future I'll make the character re-gain mana if the skill is aborted
	UE_LOG(LogTemp, Error, TEXT("Skill cast aborted!"));
	const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
	return idleState;
}

void UCastingState::OnEnter(TObjectPtr<AController> controller) {
	UE_LOG(LogTemp, Warning, TEXT("Skill cast begun!"));
	if (DisablesMovement()) {
		controller->StopMovement();
	}

	_caster = controller->GetPawn();

	const auto skill = GetSkillInExecution();

	// Even though a similar check is already present before targeting state, we need to perform it again in case something
	// is lowering the caster's mana over time while he's targeting.
	if (const auto manaC = _caster->FindComponentByClass<UManaComponent>()) {
		const double charMana = manaC->GetMana();
		const double manaCost = skill->GetCastManaCost();
		if (charMana < manaCost) {
			UE_LOG(LogTemp, Error, TEXT("Not enough mana to cast skill"));
			_bEndCasting = true;
			return;
		}
		manaC->SetMana(charMana - manaCost);
	}


	if (FMath::IsNearlyZero(skill->GetCastTime())) {
		_endCasting();
	}

	GetWorld()->GetTimerManager().SetTimer(_countdownToCast, this, &UCastingState::_endCasting, skill->GetCastTime());
}

void UCastingState::OnLeave(TObjectPtr<AController> controller) {
	if (GetWorld()->GetTimerManager().GetTimerRemaining(_countdownToCast) > 0.) {
		GetWorld()->GetTimerManager().ClearTimer(_countdownToCast);
	}
}

void UCastingState::_endCasting() {
	check(_caster.IsValid());

	UE_LOG(LogTemp, Warning, TEXT("Skill cast is over!"));
	GetSkillInExecution()->Execute(_caster.Get());
	_bEndCasting = true;
}
