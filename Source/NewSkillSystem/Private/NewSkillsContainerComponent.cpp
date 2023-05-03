// Fill out your copyright notice in the Description page of Project Settings.

#include "NewSkillsContainerComponent.h"

#include "NewAbstractSkill.h"

UNewSkillsContainerComponent::UNewSkillsContainerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

FSkillCastResult UNewSkillsContainerComponent::TryCastSkillAtIndex(const int32 index) {
    checkf(index < _skills.Num(), TEXT("There is no skill at index %i"), index);
    check(IsValid(_skills[index]));

    return _skills[index]->TryCast();
}

void UNewSkillsContainerComponent::BeginPlay() {
    Super::BeginPlay();

    const auto caster = GetOwner();
    for (const auto skill : _skills) {
        skill->SetCaster(caster);
    }
}
