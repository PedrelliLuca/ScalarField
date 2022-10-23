// Fill out your copyright notice in the Description page of Project Settings.


#include "CastingState.h"

#include "IdleState.h"
#include "ManaComponent.h"
#include "SkillsContainerComponent.h"


TObjectPtr<USkillUserState> UCastingState::OnTargeting(TObjectPtr<APlayerController> controller) {
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

	if (skill == GetSkillInExecution().Get()) {
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
		const double manaCost = skill->GetManaCost();
		if (charMana < manaCost) {
			UE_LOG(LogTemp, Error, TEXT("Not enough mana to cast skill at index %i"), index);
			return _keepCurrentState();
		}

		manaC->SetMana(charMana - manaCost);
	}
	
	// TODO: if skill requires target return a targeting state, else return a casting state
	const auto castingState = NewObject<UCastingState>(controller, UCastingState::StaticClass());
	castingState->SetSkillInExecution(skill);
	return castingState;
}

TObjectPtr<USkillUserState> UCastingState::OnTick(float deltaTime, TObjectPtr<APlayerController> controller) {
	if (_bIsCastingOver) {
		// TODO: if skill requires channeling return a channeling state, else return an idle state
		const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
		return idleState;
	}

	return _keepCurrentState();
}

void UCastingState::OnEnter(TObjectPtr<AController> controller) {
	check(GetSkillInExecution().IsValid());

	if (DisablesMovement()) {
		controller->StopMovement();
	}

	_caster = controller->GetPawn();

	const auto skill = GetSkillInExecution().Get();

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

	GetSkillInExecution().Get()->Execute(_caster.Get());
	_bIsCastingOver = true;
}
