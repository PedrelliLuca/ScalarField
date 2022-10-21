// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAbstractSkill.h"

void UNewAbstractSkill::_startCooldown() {
	check(!_bIsOnCooldown);

	_bIsOnCooldown = true;
	FTimerHandle timerHandle{};
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UNewAbstractSkill::_endCooldown, _parameters.Cooldown);
}

void UNewAbstractSkill::_endCooldown() {
	_bIsOnCooldown = false;
}
