// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillsContainerComponent.h"

#include "AbstractSkill.h"

USkillsContainerComponent::USkillsContainerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USkillsContainerComponent::CreateAllSkills() {
    const auto caster = GetOwner();
    _skills.Reserve(_skillClasses.Num());
    for (const auto& skillClass : _skillClasses) {
        auto skill = NewObject<UAbstractSkill>(this, skillClass);
        skill->SetCaster(caster);
        _skills.Emplace(MoveTemp(skill));
    }
}

FSkillCastResult USkillsContainerComponent::TryCastSkillAtIndex(const int32 index) {
    if (index >= _skills.Num()) {
        // TODO: this is a random thing, we should return a proper error in here!
        return FSkillCastResult::CastFail_CastConditionsViolated();
    }

    check(IsValid(_skills[index]));

    const auto skillCastResult = _skills[index]->TryCast();
    if (skillCastResult.IsFailure()) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillCastResult.GetErrorText().ToString());

        // In case of failure, do not override the waiting skill unless the skill that just failed requires targets.
        if (skillCastResult.GetCastResult() == ESkillCastResult::Fail_MissingTarget) {
            _resetWaitingSkill();
            _skillWaitingForTargets = _skills[index];
        }

        return skillCastResult;
    }

    // Successful cast.
    // In case some skill was being executed, we abort it. If some skill was waiting, we reset its targets.
    _resetSkillInExecution(false);
    _resetWaitingSkill();

    // Update the skill being executed with the new one
    _setNewSkillInExecution(_skills[index], skillCastResult.GetCastResult());

    return skillCastResult;
}

TOptional<FSkillCastResult> USkillsContainerComponent::TryCastWaitingSkill() {
    if (!ensureAlwaysMsgf(_skillWaitingForTargets.IsValid(), TEXT("No Waiting Skill to be cast."))) {
        return TOptional<FSkillCastResult>{};
    }

    auto skillCastResult = _skillWaitingForTargets->TryCast();
    if (skillCastResult.IsFailure()) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *skillCastResult.GetErrorText().ToString());

        // Reset every target
        _resetWaitingSkill();
        return MoveTemp(skillCastResult);
    }

    // Successful cast.
    // In case some skill was being executed, we abort it. Then, we replace the exeucting skill with the waiting skill and invalidate this latter.
    _resetSkillInExecution(false);
    _setNewSkillInExecution(_skillWaitingForTargets.Get(), skillCastResult.GetCastResult());
    _skillWaitingForTargets = nullptr;

    return MoveTemp(skillCastResult);
}

bool USkillsContainerComponent::AbortSkillInExecution() {
    bool executionSkillReset = false;
    const bool waitingSkillReset = _resetWaitingSkill();

    // We want to abort only one skill at a time: if we a skill waiting for targets and a skill in execution, only the former gets aborted.
    if (!waitingSkillReset) {
        executionSkillReset = _resetSkillInExecution(true);
    }
    return executionSkillReset || waitingSkillReset;
}

bool USkillsContainerComponent::AbortWaitingSkill() {
    return _resetWaitingSkill();
}

TOptional<FSkillTargetingResult> USkillsContainerComponent::TryAddTargetToWaitingSkill(const FSkillTargetPacket& targetPacket) {
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

void USkillsContainerComponent::_setNewSkillInExecution(const TObjectPtr<UAbstractSkill> skill, const ESkillCastResult castResultValue) {
    /* If we don't get inside the following statement, it means that either:
     * 1. The result was ESkillCastResult::Success_IntoExecutionEnd. In such case, the skill was instantaneous, we don't to cache it and bind to its delegates.
     * 2. The cast was a failure, so we don't want to set the skill as "in execution". */
    if (castResultValue == ESkillCastResult::Deferred || castResultValue == ESkillCastResult::Success_IntoChanneling) {
        _skillCurrentlyBeingExecuted = skill;
        _onSkillInExecutionStatusChanged.Broadcast(true);

        if (castResultValue == ESkillCastResult::Deferred || castResultValue == ESkillCastResult::Success_IntoChanneling) {
            _skillBeingExecutedDelegate =
                _skillCurrentlyBeingExecuted->OnSkillStatusChanged().AddUObject(this, &USkillsContainerComponent::_onCurrentlyExecutedSkillStatusChanged);
        }
    }
}

void USkillsContainerComponent::_onCurrentlyExecutedSkillStatusChanged(const ESkillStatus newStatus) {
    if (newStatus == ESkillStatus::Cooldown || newStatus == ESkillStatus::None) {
        _skillCurrentlyBeingExecuted->OnSkillStatusChanged().Remove(_skillBeingExecutedDelegate);
        _skillCurrentlyBeingExecuted = nullptr;
        _onSkillInExecutionStatusChanged.Broadcast(false);
    }
}

bool USkillsContainerComponent::_resetSkillInExecution(const bool resetMovement) {
    if (_skillCurrentlyBeingExecuted.IsValid()) {
        _skillCurrentlyBeingExecuted->Abort(resetMovement);
        return true;
    }
    return false;
}

bool USkillsContainerComponent::_resetWaitingSkill() {
    if (_skillWaitingForTargets.IsValid()) {
        _skillWaitingForTargets->Abort(false); // Targets' cleanup.
        _skillWaitingForTargets = nullptr;
        return true;
    }

    return false;
}
