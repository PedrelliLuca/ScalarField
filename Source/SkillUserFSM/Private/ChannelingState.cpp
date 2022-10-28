// Fill out your copyright notice in the Description page of Project Settings.


#include "ChannelingState.h"

#include "IdleState.h"
#include "ManaComponent.h"



TObjectPtr<USkillUserState> UChannelingState::OnTargeting(TObjectPtr<AActor> target, const TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnBeginSkillExecution(const int32 skillKey, const TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnTick(float deltaTime, const TObjectPtr<AController> controller) {
	_elapsedChannelingTime += deltaTime;
	const auto skill = GetSkillInExecution();
	const double channelingDuration = GetSkillInExecution()->GetChannelingTime();
	if (_elapsedChannelingTime >= channelingDuration) {
		// Apply one last mana payment with reduced deltaTime
		deltaTime = _elapsedChannelingTime - channelingDuration;
	}

	// No mana component? Channeling is free for _caster!
	if (const auto manaC = _caster->FindComponentByClass<UManaComponent>()) {
		const double charMana = manaC->GetMana();

		const double manaCost = skill->GetChannelingManaCost();
		const double manaCostThisFrame = (deltaTime / channelingDuration) * manaCost;

		if (charMana < manaCostThisFrame) {
			UE_LOG(LogTemp, Error, TEXT("Not enough mana to cast skill"));
			const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
			return idleState;
		}

		manaC->SetMana(charMana - manaCostThisFrame);
	}

	// Functionality of this particular skill requiring channeling
	skill->ExecuteChannelingTick(deltaTime, _caster.Get());

	if (_elapsedChannelingTime >= channelingDuration) {
		const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
		return idleState;
	}

	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnSkillExecutionAborted(const TObjectPtr<AController> controller) {
	UE_LOG(LogTemp, Error, TEXT("Skill channeling aborted!"));

	GetSkillInExecution()->RemoveAllTargets();
	const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
	return idleState;
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
