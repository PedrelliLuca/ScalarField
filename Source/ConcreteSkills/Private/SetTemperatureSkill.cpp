// Fill out your copyright notice in the Description page of Project Settings.

#include "SetTemperatureSkill.h"

#include "SkillTargets/ActorSkillTarget.h"
#include "ThermodynamicsInteractorComponent.h"

void USetTemperatureSkill::_skillCast() {
    auto const& targets = _getTargets();

    check(!targets.IsEmpty());

    // TODO: generalize this in the future.
    auto const abstractTarget = targets[0];
    auto const skillTarget = Cast<UActorSkillTarget>(abstractTarget.GetObject());
    check(IsValid(skillTarget));
    auto const target = skillTarget->GetActor();

    auto thermoC = target->FindComponentByClass<UThermodynamicsInteractorComponent>();

    // If this triggers, it means you didn't set up the skill targeting conditions properly. This skill targets should have a thermodynamic component.
    check(IsValid(thermoC));
    thermoC->SetTemperature(_temperatureToSet);
}

void USetTemperatureSkill::_skillChannelingTick(float const deltaTime) {
}

void USetTemperatureSkill::_skillAbort() {
}
