// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillsContainerComponent.h"

USkillsContainerComponent::USkillsContainerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

TObjectPtr<UAbstractSkill> USkillsContainerComponent::GetSkillAtIndex(const uint32 index) const {
    if (_skills.IsValidIndex(index)) {
        check(IsValid(_skills[index]));
        return _skills[index];
    }
    return nullptr;
}

TObjectPtr<UAbstractSkill> USkillsContainerComponent::FindSkillByClass(TSubclassOf<UAbstractSkill> skillClass) const {
    const auto skillOfClass = _skills.FindByPredicate([&skillClass](TObjectPtr<UAbstractSkill> skill) { return skill->IsA(skillClass); });

    return skillOfClass != nullptr ? *skillOfClass : nullptr;
}

void USkillsContainerComponent::BeginPlay() {
    Super::BeginPlay();

    for (const auto skillClass : _skillsClasses) {
        const auto skill = NewObject<UAbstractSkill>(GetOwner(), skillClass);

#if DO_CHECK
        skill->CheckParametersSanity();
#endif

        _skills.Emplace(skill);
    }
}
