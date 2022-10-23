// Fill out your copyright notice in the Description page of Project Settings.

#include "CarbonizeSkill.h"
#include "ThermodynamicComponent.h"

void UCarbonizeSkill::Execute(const TObjectPtr<AActor> caster) {
	const auto target = _getActorTargets()[0].Target;
	const auto thermoC = target->FindComponentByClass<UThermodynamicComponent>();
	check(IsValid(thermoC));

	thermoC->SetTemperature(_carbonizationTemperature);

	_startCooldown();
}

#if DO_CHECK
void UCarbonizeSkill::CheckParametersSanity() const {
	check(NumberOfTargets() == 1);
}
#endif
