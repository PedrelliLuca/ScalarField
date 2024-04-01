// Fill out your copyright notice in the Description page of Project Settings.

#include "SetTemperatureSkill.h"

#include "SkillTargets/ActorSkillTarget.h"
#include "ThermodynamicsInteractorComponent.h"

void USetTemperatureSkill::_skillCast() {
    const auto& targets = _getTargets();

    check(!targets.IsEmpty());

    // TODO: generalize this in the future.
    const auto abstractTarget = targets[0];
    const auto skillTarget = Cast<UActorSkillTarget>(abstractTarget.GetObject());
    check(IsValid(skillTarget));
    const auto target = skillTarget->GetActor();

    auto thermoC = target->FindComponentByClass<UThermodynamicsInteractorComponent>();

    // If this triggers, it means you didn't set up the skill targeting conditions properly. This skill targets should have a thermodynamic component.
    check(IsValid(thermoC));
    thermoC->SetTemperature(_temperatureToSet);
}

void USetTemperatureSkill::_skillChannelingTick(const float deltaTime) {
}

void USetTemperatureSkill::_skillAbort() {
}
