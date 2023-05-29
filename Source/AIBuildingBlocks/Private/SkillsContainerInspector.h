// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NewAbstractSkill.h"
#include "NewSkillsContainerComponent.h"
#include "SkillPropertiesInspector.h"

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

    TOptional<int32> GetIndexOfSkill(const TSubclassOf<UNewAbstractSkill>& skillClass) const {
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