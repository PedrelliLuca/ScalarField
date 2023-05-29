// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NewAbstractSkill.h"

class FSkillPropertiesInspector {
public:
    FSkillPropertiesInspector(const TObjectPtr<UNewAbstractSkill>& skill)
        : _skill(skill) {}

    float GetTotalManaCost() const { return _skill->_castManaCost + _skill->_channelingManaCost; }

private:
    TWeakObjectPtr<UNewAbstractSkill> _skill;
};