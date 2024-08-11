// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillPropertiesInspector.h"

#include "AbstractSkill.h"

FSkillPropertiesInspector::FSkillPropertiesInspector(const TObjectPtr<UAbstractSkill>& skill)
    : _skill(skill) {
}

float FSkillPropertiesInspector::GetTotalManaCost() const {
    check(_skill.IsValid());
    return _skill->_castManaCost + _skill->_channelingManaCost;
}

float FSkillPropertiesInspector::GetCooldownSeconds() const {
    check(_skill.IsValid());
    return _skill->_cooldownSeconds;
}

TSubclassOf<UObject> FSkillPropertiesInspector::GetTargetKind() const {
    check(_skill.IsValid());
    return _skill->_targetKind;
}

const TArray<TObjectPtr<USkillTargetingCondition>>& FSkillPropertiesInspector::GetTargetingConditions() const {
    check(_skill.IsValid());
    return _skill->_targetingConditions;
}

FOnSkillStatusChanged& FSkillPropertiesInspector::OnSkillStatusChanged() const {
    check(_skill.IsValid());
    return _skill->_onSkillStatusChanged;
}
