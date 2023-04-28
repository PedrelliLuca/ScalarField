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

void UAbstractSkill::SetCaster(const TObjectPtr<AActor>& caster) {
    // Make sure that the caster is not valid before setting it.
    if (ensureMsgf(!_caster.IsValid(), TEXT("Caster can be set only once and has already been set."))) {
        check(IsValid(caster));
        _caster = caster;

        for (const auto castCondition : _castConditions) {
            castCondition->SetConditionSubject(caster);
        }
    }
}

bool UAbstractSkill::IsValidTarget(const int32 index, const TObjectPtr<AActor> target) const {
    check(_parameters.ActorTargetParameters.IsValidIndex(index));

    /* TODO: implement a command pattern to test the skill validity: _parameters has an array of ITargetCondition objects
     * with a IsConditionVerified() virtual pure function, and this interface is implemented by the true conditions like
     * "are the components correct", "is the target an enemy?", "Is the target below this amount of health" and so on.
     * This will make unnecessary for IsValidTarget() to be virtual. Moreover, the same spell could have different
     * conditions depending on its caster.
     */

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

void UAbstractSkill::_removeAllTargets() {
    for (auto targetParams : _parameters.ActorTargetParameters) {
        targetParams.Target = nullptr;
    }
}
