// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/AbstractSkill.h"

void UAbstractSkill::StartCooldown() {
	check(!_bIsOnCooldown);

	_bIsOnCooldown = true;
	FTimerHandle timerHandle{};
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UAbstractSkill::_terminateCooldown, _parameters.CoolDown);
}