// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillsContainerInspector.h"

#include "AbstractSkill.h"
#include "SkillPropertiesInspector.h"
#include "SkillsContainerComponent.h"

FSkillsContainerInspector::FSkillsContainerInspector(const TObjectPtr<USkillsContainerComponent>& skillsContainer)
    : _skillsContainer(skillsContainer) {
}

TOptional<FSkillPropertiesInspector> FSkillsContainerInspector::GetSkillPropertiesByIndex(const int32 skillIndex) const {
    auto optionalSkillInsp = TOptional<FSkillPropertiesInspector>();
    if (_skillsContainer->_skills.IsValidIndex(skillIndex) && IsValid(_skillsContainer->_skills[skillIndex])) {
        optionalSkillInsp = FSkillPropertiesInspector(_skillsContainer->_skills[skillIndex]);
    }

    return optionalSkillInsp;
}

TOptional<int32> FSkillsContainerInspector::GetIndexOfSkill(const TSubclassOf<UAbstractSkill>& skillClass) const {
    auto optionalIndex = TOptional<int32>{};

    const auto index = _skillsContainer->_skills.IndexOfByPredicate([&skillClass](const auto& skill) { return skill->IsA(skillClass); });
    if (index != INDEX_NONE) {
        optionalIndex = index;
    }

    return optionalIndex;
}

void FSkillsContainerInspector::ForEachSkill(const TFunction<void(const FSkillPropertiesInspector&)> skillPropertiesFunc) const {
    for (TObjectPtr<UAbstractSkill> skill : _skillsContainer->_skills) {
        auto skillPropertiesIns = FSkillPropertiesInspector(skill);
        skillPropertiesFunc(skillPropertiesIns);
    }
}
