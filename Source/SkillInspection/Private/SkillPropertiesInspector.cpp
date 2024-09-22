// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillPropertiesInspector.h"

#include "AbstractSkill.h"

FSkillPropertiesInspector::FSkillPropertiesInspector(TObjectPtr<UAbstractSkill> const& skill)
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

TObjectPtr<UTexture2D> FSkillPropertiesInspector::GetSkillThumbnail() const {
    return _skill->_skillThumbnail;
}

TSubclassOf<UObject> FSkillPropertiesInspector::GetTargetKind() const {
    check(_skill.IsValid());
    return _skill->_targetKind;
}

TArray<TObjectPtr<USkillTargetingCondition>> const& FSkillPropertiesInspector::GetTargetingConditions() const {
    check(_skill.IsValid());
    return _skill->_targetingConditions;
}

FOnSkillStatusChanged& FSkillPropertiesInspector::OnSkillStatusChanged() const {
    check(_skill.IsValid());
    return _skill->_onSkillStatusChanged;
}
