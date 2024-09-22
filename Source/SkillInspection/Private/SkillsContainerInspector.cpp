// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillsContainerInspector.h"

#include "AbstractSkill.h"
#include "SkillPropertiesInspector.h"
#include "SkillsContainerComponent.h"

FSkillsContainerInspector::FSkillsContainerInspector(TObjectPtr<USkillsContainerComponent> const& skillsContainer)
    : _skillsContainer(skillsContainer) {
}

TOptional<FSkillPropertiesInspector> FSkillsContainerInspector::GetSkillPropertiesByIndex(int32 const skillIndex) const {
    auto optionalSkillInsp = TOptional<FSkillPropertiesInspector>();
    if (_skillsContainer->_skills.IsValidIndex(skillIndex) && IsValid(_skillsContainer->_skills[skillIndex])) {
        optionalSkillInsp = FSkillPropertiesInspector(_skillsContainer->_skills[skillIndex]);
    }

    return optionalSkillInsp;
}

TOptional<int32> FSkillsContainerInspector::GetIndexOfSkill(TSubclassOf<UAbstractSkill> const& skillClass) const {
    auto optionalIndex = TOptional<int32>{};

    auto const index = _skillsContainer->_skills.IndexOfByPredicate([&skillClass](auto const& skill) { return skill->IsA(skillClass); });
    if (index != INDEX_NONE) {
        optionalIndex = index;
    }

    return optionalIndex;
}

void FSkillsContainerInspector::ForEachSkill(TFunction<void(FSkillPropertiesInspector const&, int32)> const skillPropertiesFunc) const {
    check(_skillsContainer.IsValid());

    int32 skillIndex = 0;
    for (TObjectPtr<UAbstractSkill> skill : _skillsContainer->_skills) {
        auto skillPropertiesIns = FSkillPropertiesInspector(skill);
        skillPropertiesFunc(skillPropertiesIns, skillIndex);
        ++skillIndex;
    }
}
