// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractSkill.h"

void UAbstractSkill::_startCooldown() {
	check(!_bIsOnCooldown);

	_bIsOnCooldown = true;
	FTimerHandle timerHandle{};
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UAbstractSkill::_endCooldown, _parameters.Cooldown);
}

void UAbstractSkill::_endCooldown() {
	_bIsOnCooldown = false;
}
