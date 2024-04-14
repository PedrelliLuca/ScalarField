// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "NewSkillsContainerComponent.h"
#include "SkillPropertiesInspector.h"

/** \brief This class is friend of UNewSkillsContainerComponent. Files who have visibility of this class are allowed to peek at UNewSkillsContainerComponent's
 * properties. The reasons this class exists are:
 * 1. To give AI knowledge on all the possible skills at its disposal, so that it can take decisions on what to do.
 * 2. To have the UI w all the possible skills at its disposal, so that the player can take decisions on what to do.
 *
 * IMPORTANT: THIS CLASS SHOULD BE UNKNOWN BY EVERY FILE THAT IS NOT AI OR UI RELATED.
 *
 * TODO: move this in a more abstract module when the skills-related UI will be implemented.
 */
class FSkillsContainerInspector {
public:
    FSkillsContainerInspector(const TObjectPtr<UNewSkillsContainerComponent>& skillsContainer)
        : _skillsContainer(skillsContainer) {}

    TOptional<FSkillPropertiesInspector> GetSkillPropertiesByIndex(const int32 skillIndex) const {
        if (!_skillsContainer->_skills.IsValidIndex(skillIndex) || !IsValid(_skillsContainer->_skills[skillIndex])) {
            return TOptional<FSkillPropertiesInspector>{};
        }

        return FSkillPropertiesInspector{_skillsContainer->_skills[skillIndex]};
    }

    TOptional<int32> GetIndexOfSkill(const TSubclassOf<UAbstractSkill>& skillClass) const {
        auto optionalIndex = TOptional<int32>{};

        const auto index = _skillsContainer->_skills.IndexOfByPredicate([&skillClass](const auto& skill) { return skill->IsA(skillClass); });
        if (index != INDEX_NONE) {
            optionalIndex = index;
        }

        return optionalIndex;
    }

private:
    TWeakObjectPtr<UNewSkillsContainerComponent> _skillsContainer = nullptr;
};
