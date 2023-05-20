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
    if (skillCastResult.IsFailure()) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillCastResult.GetErrorText().ToString());

        if (skillCastResult.GetCastResult() == ESkillCastResult::Fail_MissingTarget) {
            _resetWaitingSkill();
            _skillWaitingForTargets = _skills[index];
        }

        return skillCastResult;
    }

    // The cast didn't result in a failure. In case some skill was being executed, we abort it. If some skill was waiting, we reset its targets.
    _resetSkillInExecution(false);
    _resetWaitingSkill();

    // Update the skill being executed with the new one
    _setNewSkillInExecution(_skills[index], skillCastResult.GetCastResult());

    return skillCastResult;
}

TOptional<FSkillCastResult> UNewSkillsContainerComponent::TryCastWaitingSkill() {
    if (!ensureAlwaysMsgf(_skillWaitingForTargets.IsValid(), TEXT("No Waiting Skill to be cast."))) {
        return TOptional<FSkillCastResult>{};
    }

    auto skillCastResult = _skillWaitingForTargets->TryCast();
    if (skillCastResult.IsFailure()) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillCastResult.GetErrorText().ToString());

        // We do not reset the targets we already acquired, that can only happen by succeeding in casting
        return MoveTemp(skillCastResult);
    }

    _resetSkillInExecution(false);
    _setNewSkillInExecution(_skillWaitingForTargets.Get(), skillCastResult.GetCastResult());
    _skillWaitingForTargets = nullptr;

    return MoveTemp(skillCastResult);
}

bool UNewSkillsContainerComponent::AbortSkillInExecution() {
    // TODO: also abort waiting skill? If not, how do we abort waiting skill?
    // _resetWaitingSkill();
    return _resetSkillInExecution(true);
}

TOptional<FSkillTargetingResult> UNewSkillsContainerComponent::TryAddTargetToWaitingSkill(const FSkillTargetPacket& targetPacket) {
    if (!_skillWaitingForTargets.IsValid()) {
        // No skill is waiting for targets
        return TOptional<FSkillTargetingResult>{};
    }

    auto skillTargetingResult = _skillWaitingForTargets->TryAddTarget(targetPacket);
    const auto targetingResult = skillTargetingResult.GetTargetingResult();

    if (skillTargetingResult.IsFailure()) {
        // If every target has already been provided to the skill we expect _skillWaitingForTargets to be nullptr.
        check(targetingResult != ESkillTargetingResult::Fail_AlreadyAvailableTargets);
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillTargetingResult.GetErrorText().ToString());
    }

    return MoveTemp(skillTargetingResult);
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

void UNewSkillsContainerComponent::_setNewSkillInExecution(const TObjectPtr<UNewAbstractSkill> skill, const ESkillCastResult castResultValue) {
    /* If we don't get inside the following statement, it means that either:
     * 1. The result was ESkillCastResult::Success_IntoExecutionEnd. In such case, the skill was instantaneous, we don't to cache it and bind to its delegates.
     * 2. The cast was a failure, so we don't want to set the skill as "in execution". */
    if (castResultValue == ESkillCastResult::Deferred || castResultValue == ESkillCastResult::Success_IntoChanneling) {
        _currentlyExecutedSkill = skill;

        if (castResultValue == ESkillCastResult::Deferred) {
            _currentlyExecutedSkill->OnCastPhaseEnd().AddUObject(this, &UNewSkillsContainerComponent::_onCurrentlyExecutedSkillCastPhaseEnd);
        }
        _currentlyExecutedSkill->OnChannelingPhaseEnd().AddUObject(this, &UNewSkillsContainerComponent::_onCurrentlyExecutedSkillChannelingPhaseEnd);
    }
}

void UNewSkillsContainerComponent::_onCurrentlyExecutedSkillCastPhaseEnd(const FSkillCastResult skillCastResult) {
    const auto castResultValue = skillCastResult.GetCastResult();

    // Success_IntoChanneling is the only result doesn't determine the end of the current skill execution.
    if (skillCastResult.IsFailure()) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillCastResult.GetErrorText().ToString());
    }

    if (castResultValue != ESkillCastResult::Success_IntoChanneling) {
        _currentlyExecutedSkill = nullptr;
    }
}

void UNewSkillsContainerComponent::_onCurrentlyExecutedSkillChannelingPhaseEnd(const FSkillChannelingResult skillChannelingResult) {
    if (skillChannelingResult.IsFailure()) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillChannelingResult.GetErrorText().ToString());
    }
    _currentlyExecutedSkill = nullptr;
}

bool UNewSkillsContainerComponent::_resetSkillInExecution(const bool resetMovement) {
    if (_currentlyExecutedSkill.IsValid()) {
        _currentlyExecutedSkill->Abort(resetMovement);
        _currentlyExecutedSkill = nullptr;
        return true;
    }
    return false;
}

void UNewSkillsContainerComponent::_resetWaitingSkill() {
    if (_skillWaitingForTargets.IsValid()) {
        _skillWaitingForTargets->Abort(false); // Targets' cleanup.
        _skillWaitingForTargets = nullptr;
    }
}
