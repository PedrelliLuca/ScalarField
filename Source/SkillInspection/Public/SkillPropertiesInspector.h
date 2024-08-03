// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UAbstractSkill;
class USkillTargetingCondition;

/** \brief This class is friend of UAbstractSkill. Files who have visibility of this class are allowed to peek at UAbstractSkill's properties. The reasons
 * this class exists are:
 * 1. To provide AI with skill-related data, so that it can take decisions on whether to cast a certain skill or not.
 * 2. To provide UI with skill-related data, so that the player can take decisions on whether to cast a certain skill or not.
 *
 *
 * IMPORTANT: THIS CLASS SHALL BE HIDDEN TO EVERY FILE THAT IS NOT AI OR UI RELATED.
 */
class SKILLINSPECTION_API FSkillPropertiesInspector {
public:
    FSkillPropertiesInspector(const TObjectPtr<UAbstractSkill>& skill);

    float GetTotalManaCost() const;

    TSubclassOf<UObject> GetTargetKind() const;

    const TArray<TObjectPtr<USkillTargetingCondition>>& GetTargetingConditions() const;

private:
    TWeakObjectPtr<UAbstractSkill> _skill;
};
