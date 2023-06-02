// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NewAbstractSkill.h"

/** \brief This class is friend of UNewAbstractSkill. Files who have visibility of this class are allowed to peek at UNewAbstractSkill's properties. The reasons
 * this class exists are:
 * 1. To provide AI with skill-related data, so that it can take decisions on whether to cast a certain skill or not.
 * 2. To provide UI with skill-related data, so that the player can take decisions on whether to cast a certain skill or not.
 *
 *
 * IMPORTANT: THIS CLASS SHOULD BE UNKNOWN BY EVERY FILE THAT IS NOT AI OR UI RELATED.
 *
 * TODO: move this in a more abstract module when the skills-related UI will be implemented.
 */
class FSkillPropertiesInspector {
public:
    FSkillPropertiesInspector(const TObjectPtr<UNewAbstractSkill>& skill)
        : _skill(skill) {}

    float GetTotalManaCost() const { return _skill->_castManaCost + _skill->_channelingManaCost; }

    TSubclassOf<UObject> GetTargetKind() const { return _skill->_targetKind; }

private:
    TWeakObjectPtr<UNewAbstractSkill> _skill;
};