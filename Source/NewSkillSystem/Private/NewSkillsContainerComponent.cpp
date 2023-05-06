// Fill out your copyright notice in the Description page of Project Settings.

#include "NewSkillsContainerComponent.h"

#include "NewAbstractSkill.h"

UNewSkillsContainerComponent::UNewSkillsContainerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

FSkillCastResult UNewSkillsContainerComponent::TryCastSkillAtIndex(const int32 index) {
    checkf(index < _skills.Num(), TEXT("There is no skill at index %i"), index);
    check(IsValid(_skills[index]));

    const auto skillCastResult = _skills[index]->TryCast();
    if (!skillCastResult.IsFailure()) {
        if (_currentlyExecutedSkill.IsValid()) {
            _currentlyExecutedSkill->Abort();
        }
        const auto castResultValue = skillCastResult.GetCastResult();
        if (castResultValue == ESkillCastResult::Deferred || castResultValue == ESkillCastResult::Success_IntoChanneling) {
            _currentlyExecutedSkill = _skills[index];

            // You don't need to handle the removal from these delegates, UAbstractSkill automatically takes care of that.
            _currentlyExecutedSkill->OnCastPhaseEnd().AddUObject(this, &UNewSkillsContainerComponent::_onCurrentlyExecutedSkillCastPhaseEnd);
            _currentlyExecutedSkill->OnChannelingPhaseEnd().AddUObject(this, &UNewSkillsContainerComponent::_onCurrentlyExecutedSkillChannelingPhaseEnd);
        }
    }

    return skillCastResult;
}

void UNewSkillsContainerComponent::BeginPlay() {
    Super::BeginPlay();

    const auto caster = GetOwner();
    _skills.Reserve(_skillClasses.Num());
    for (const auto& skillClass : _skillClasses) {
        auto skill = NewObject<UNewAbstractSkill>(this, skillClass);
        skill->SetCaster(caster);
        _skills.Emplace(MoveTemp(skill));
    }
}

void UNewSkillsContainerComponent::_onCurrentlyExecutedSkillCastPhaseEnd(const FSkillCastResult skillCastResult) {
    const auto castResultValue = skillCastResult.GetCastResult();

    // Success_IntoChanneling is the only result doesn't determine the end of the current skill execution.
    if (castResultValue != ESkillCastResult::Success_IntoChanneling) {
        _currentlyExecutedSkill = nullptr;
    }
}

void UNewSkillsContainerComponent::_onCurrentlyExecutedSkillChannelingPhaseEnd(const FSkillChannelingResult skillChannelingResult) {
    _currentlyExecutedSkill = nullptr;
}
