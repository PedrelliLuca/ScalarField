// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingConditions/DistanceTargetingCondition.h"

bool UDistanceTargetingCondition::IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const {
    check(_caster.IsValid());

    const auto casterLocation = _caster->GetActorLocation();
    const auto targetLocation = skillTarget->GetTargetLocation();

    const auto casterToTarget = targetLocation - casterLocation;
    const auto actualDistance = casterToTarget.Length();

    bool isVerified = false;
    switch (_distanceComparison) {
        case EDistanceComparisonStyle::DCS_Equality:
            isVerified = FMath::IsNearlyEqual(actualDistance, _targetingConditionDistance);
            break;
        case EDistanceComparisonStyle::DCS_LessThan:
            isVerified = actualDistance < _targetingConditionDistance;
            break;
        case EDistanceComparisonStyle::DCS_GreaterThan:
            isVerified = actualDistance > _targetingConditionDistance;
            break;
        default:
            checkNoEntry();
    };

    return isVerified;
}

void UDistanceTargetingCondition::SetSkillCaster(TObjectPtr<AActor> skillCaster) {
    // Make sure that the caster is not valid before setting it.
    if (ensureMsgf(!_caster.IsValid(), TEXT("Condition Caster can be set only once and has already been set."))) {
        check(IsValid(skillCaster));
        _caster = MoveTemp(skillCaster);
    }
}
