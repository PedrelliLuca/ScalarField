// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingConditions/DistanceTargetingCondition.h"

bool UDistanceTargetingCondition::IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const {
    return false;
}

void UDistanceTargetingCondition::SetSkillCaster(TObjectPtr<AActor> skillCaster) {
}
