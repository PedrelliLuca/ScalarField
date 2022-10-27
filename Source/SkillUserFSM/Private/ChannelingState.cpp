// Fill out your copyright notice in the Description page of Project Settings.


#include "ChannelingState.h"

#include "IdleState.h"
#include "ManaComponent.h"



TObjectPtr<USkillUserState> UChannelingState::OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnBeginSkillExecution(const int32 skillKey, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnTick(float deltaTime, TObjectPtr<AController> controller) {
	if (_bEndChanneling) {
		const auto idleState = NewObject<UIdleState>(controller, UIdleState::StaticClass());
		return idleState;
	}

	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnSkillExecutionAborted(TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

void UChannelingState::OnEnter(TObjectPtr<AController> controller) {
	const auto skill = GetSkillInExecution();

	check(IsValid(skill) && skill->RequiresChanneling() && skill->GetChannelingManaCost() > 0.);
	UE_LOG(LogTemp, Warning, TEXT("Channeling..."));

	if (DisablesMovement()) {
		controller->StopMovement();
	}

	_caster = controller->GetPawn();

	GetWorld()->GetTimerManager().SetTimer(_channelingTimer, this, &UChannelingState::_endChanneling, skill->GetChannelingTime());
}

void UChannelingState::OnLeave(TObjectPtr<AController> controller) {
}

void UChannelingState::_endChanneling() {
	check(_caster.IsValid());
	UE_LOG(LogTemp, Warning, TEXT("Skill cast is over!"));
	_bEndChanneling = true;
}
