// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/BaseSkill.h"

void UBaseSkill::StartCooldown() {
	check(!_bIsOnCooldown);

	_bIsOnCooldown = true;
	FTimerHandle timerHandle{};
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UBaseSkill::_terminateCooldown, _parameters.CoolDown);
}