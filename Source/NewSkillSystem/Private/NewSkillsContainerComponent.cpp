// Fill out your copyright notice in the Description page of Project Settings.

#include "NewSkillsContainerComponent.h"

#include "NewAbstractSkill.h"

UNewSkillsContainerComponent::UNewSkillsContainerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

bool UNewSkillsContainerComponent::TryCastSkillAtIndex(const uint32 index) {
    checkf(index < _skills.Num(), TEXT("There is no skill at index %i"), index);
    _skills[index]->TryCast();
    return false;
}

void UNewSkillsContainerComponent::BeginPlay() {
    Super::BeginPlay();

    const auto caster = GetOwner();
    for (const auto skill : _skills) {
        // skill->SetCaster(caster);
    }
}
