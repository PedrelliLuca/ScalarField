// Fill out your copyright notice in the Description page of Project Settings.

#include "NewSkillsContainerComponent.h"

#include "NewAbstractSkill.h"

bool UNewSkillsContainerComponent::TryExecuteSkillAtIndex(const uint32 index) {
    if (ensureMsgf(index < _skills.Num(), TEXT("There is no skill at index %i"), index)) {
        // Returns whether the number of targets is ok or not, the conditions on the caster are ok or not,
        _skills[index]->Cast();
    }
    return false;
}

void UNewSkillsContainerComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    TArray<TWeakObjectPtr<UNewAbstractSkill>> skillsToRemoveFromCooldown;
    for (auto& [skill, cooldown] : _skillsToCooldowns) {
        cooldown -= deltaTime;

        // Keep track of those skills that aren't in cooldown anymore
        if (cooldown < 0.0f || FMath::IsNearlyZero(cooldown)) {
            skillsToRemoveFromCooldown.Emplace(skill);
        }
    }

    // Remove from the map the skills that aren't in cooldown
    for (const auto& skillToRemove : skillsToRemoveFromCooldown) {
        _skillsToCooldowns.Remove(skillToRemove);
    }
}

void UNewSkillsContainerComponent::BeginPlay() {
    Super::BeginPlay();

    const auto caster = GetOwner();
    for (const auto skill : _skills) {
        // skill->SetCaster(caster);
    }
}
