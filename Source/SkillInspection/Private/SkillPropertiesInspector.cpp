// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillPropertiesInspector.h"

#include "AbstractSkill.h"

FSkillPropertiesInspector::FSkillPropertiesInspector(const TObjectPtr<UAbstractSkill>& skill)
    : _skill(skill) {
}

float FSkillPropertiesInspector::GetTotalManaCost() const {
    return _skill->_castManaCost + _skill->_channelingManaCost;
}

TSubclassOf<UObject> FSkillPropertiesInspector::GetTargetKind() const {
    return _skill->_targetKind;
}

const TArray<TObjectPtr<USkillTargetingCondition>>& FSkillPropertiesInspector::GetTargetingConditions() const {
    return _skill->_targetingConditions;
}
