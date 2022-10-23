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

bool UAbstractSkill::IsValidTarget(const int32 index, const TObjectPtr<AActor> target) const {
	check(_parameters.ActorTargetParameters.IsValidIndex(index));

	const auto targetParams = _parameters.ActorTargetParameters[index];
	for (const auto componentClass : targetParams.RequiredComponents) {
		if (!target->FindComponentByClass(componentClass)) {
			return false;
		}
	}

	return true;
}

double UAbstractSkill::GetMaxDistanceForTarget(int32 targetIndex) const {
	check(_parameters.ActorTargetParameters.IsValidIndex(targetIndex));
	return _parameters.ActorTargetParameters[targetIndex].MaxDistance;
}

void UAbstractSkill::SetTarget(int32 index, TWeakObjectPtr<AActor> target) {
	check(_parameters.ActorTargetParameters.IsValidIndex(index));
	_parameters.ActorTargetParameters[index].Target = target;
}

void UAbstractSkill::RemoveAllTargets() {
	for (auto targetParams : _parameters.ActorTargetParameters) {
		targetParams.Target = nullptr;
	}
}