// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingConditions/IgnitableTargetingCondition.h"

bool UIgnitableTargetingCondition::IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const {
    return false;
}

void UIgnitableTargetingCondition::SetSkillCaster(TObjectPtr<AActor> skillCaster) {
}
