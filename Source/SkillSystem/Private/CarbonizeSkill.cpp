// Fill out your copyright notice in the Description page of Project Settings.

#include "CarbonizeSkill.h"

#include "MaterialsContainerComponent.h"
#include "ThermodynamicComponent.h"

void UCarbonizeSkill::Execute(const TObjectPtr<AActor> caster) {
	const auto target = _getActorTargets()[0].Target;

	const auto materialsC = target->FindComponentByClass<UMaterialsContainerComponent>();
	check(IsValid(materialsC));

	const auto thermoC = target->FindComponentByClass<UThermodynamicComponent>();
	check(IsValid(thermoC));

	thermoC->SetTemperature(_carbonizationTemperature);

	_startCooldown();
}

bool UCarbonizeSkill::IsValidTarget(const int32 targetIndex, const TObjectPtr<AActor> target) const
{
	if (!Super::IsValidTarget(targetIndex, target)) {
		return false;
	}

	const auto materialsC = target->FindComponentByClass<UMaterialsContainerComponent>();
	return materialsC->HasMaterial(EMaterial::M_Carbon);
}

#if DO_CHECK
void UCarbonizeSkill::CheckParametersSanity() const {
	check(NumberOfTargets() == 1);
}
#endif
