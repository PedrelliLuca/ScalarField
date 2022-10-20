// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAbstractSkill.h"

void UNewAbstractSkill::InitializeSkill(const TWeakObjectPtr<UNewSkillParameters> skillParameters) {
	check(skillParameters.IsValid());
	
	const TObjectPtr<UNewSkillParameters> skillParams = skillParameters.Get();
	_manaCost = skillParams->ManaCost;
	_cooldown = skillParams->Cooldown;
	_castTime = skillParams->CastTime;
	_channelingTime = skillParams->ChannelingTime;
	_duration = skillParams->Duration;
}

void UNewAbstractSkill::_startCooldown() {
	check(!_bIsOnCooldown);

	_bIsOnCooldown = true;
	FTimerHandle timerHandle{};
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UNewAbstractSkill::_endCooldown, _cooldown);
}

void UNewAbstractSkill::_endCooldown() {
	_bIsOnCooldown = false;
}