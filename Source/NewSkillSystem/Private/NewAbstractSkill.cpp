// Fill out your copyright notice in the Description page of Project Settings.

#include "NewAbstractSkill.h"

FSkillCastResult UNewAbstractSkill::TryCast() {
    if (_onCooldown) {
        return FSkillCastResult::CastFail_Cooldown();
    }

    // TODO: Target-based conditions

    // TODO: call this at abort and when the cast is over, not when it starts!
    GetWorld()->GetTimerManager().SetTimer(_cooldownTimer, this, &UNewAbstractSkill::_onCooldownEnded, _cooldown, false);
    return FSkillCastResult::CastSuccess();
}

void UNewAbstractSkill::SetCaster(TObjectPtr<AActor> caster) {
    // Make sure that the caster is not valid before setting it.
    if (ensureMsgf(!_caster.IsValid(), TEXT("Caster can be set only once and has already been set."))) {
        check(IsValid(caster));
        _caster = caster;

        // TODO: implement cast conditions
        /*for (const auto castCondition : _castConditions) {
            castCondition->SetConditionSubject(caster);
        }*/
    }
}

void UNewAbstractSkill::_onCooldownEnded() {
    _onCooldown = false;
}
