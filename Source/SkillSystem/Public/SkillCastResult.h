// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Represents the possible outcomes of a skill execution request.
enum class ESkillCastResult : uint8
{
    None,
    Fail_OnCooldown,
    Fail_MissingTarget,
    Fail_TargetingConditionsViolated,
    Fail_CastConditionsViolated,
    Fail_InExecution,
    Fail_InsufficientMana,
    Deferred,
    Success_IntoChanneling,
    Success_IntoExecutionEnd,
};

// Struct that collects the enum representing the cast request outcome and the error associated with it
// TODO: The error is just a message for the time being, but in the future a more refined error management will be needed.
class FSkillCastResult {
public:
    // Functions to standardize instances of this class
    static FSkillCastResult CastFail_Cooldown();
    static FSkillCastResult CastFail_MissingTarget();
    static FSkillCastResult CastFail_TargetingConditionsViolated();
    static FSkillCastResult CastFail_CastConditionsViolated();
    static FSkillCastResult CastFail_InExecution();
    static FSkillCastResult CastFail_InsufficientMana();
    static FSkillCastResult CastDeferred();
    static FSkillCastResult CastSuccess_IntoChanneling();
    static FSkillCastResult CastSuccess_IntoExecutionEnd();

    ESkillCastResult GetCastResult() const { return _result; }

    const FText& GetErrorText() const { return _errorText; }

    bool IsFailure() const { return !_errorText.IsEmpty(); }

private:
    FSkillCastResult(ESkillCastResult result, FText&& errorText)
        : _result(result)
        , _errorText(MoveTemp(errorText)) {}

    ESkillCastResult _result = ESkillCastResult::None;

    FText _errorText{};
};

#define LOCTEXT_NAMESPACE "SkillCastResult"

FORCEINLINE FSkillCastResult FSkillCastResult::CastFail_Cooldown() {
    constexpr auto result = ESkillCastResult::Fail_OnCooldown;
    auto errorText = LOCTEXT("SkillCastResult_OnCooldownText", "Couldn't cast skill, it's on cooldown.");

    return FSkillCastResult{result, MoveTemp(errorText)};
}

// TODO: in the future, this could take an integer representing the number of missing targets, so that the message can be more meaningful.
FORCEINLINE FSkillCastResult FSkillCastResult::CastFail_MissingTarget() {
    constexpr auto result = ESkillCastResult::Fail_MissingTarget;
    auto errorText = LOCTEXT("SkillCastResult_MissingTargetText", "Couldn't cast skill, targets are missing.");

    return FSkillCastResult{result, MoveTemp(errorText)};
}

// TODO: in the future, this will take an array of errors coming from the violated cast conditions, so that the message can be more meaningful.
FORCEINLINE FSkillCastResult FSkillCastResult::CastFail_TargetingConditionsViolated() {
    constexpr auto result = ESkillCastResult::Fail_TargetingConditionsViolated;
    auto errorText = LOCTEXT("SkillCastResult_TargetingConditionsViolatedText", "Couldn't cast skill, some condition on a target is vioalted.");
    return FSkillCastResult(result, MoveTemp(errorText));
}

// TODO: in the future, this will take an array of errors coming from the violated cast conditions, so that the message can be more meaningful.
FORCEINLINE FSkillCastResult FSkillCastResult::CastFail_CastConditionsViolated() {
    constexpr auto result = ESkillCastResult::Fail_CastConditionsViolated;
    auto errorText = LOCTEXT("SkillCastResult_CastConditionsViolatedText", "Couldn't cast skill, some condition on caster is vioalted.");
    return FSkillCastResult(result, MoveTemp(errorText));
}

FORCEINLINE FSkillCastResult FSkillCastResult::CastFail_InExecution() {
    constexpr auto result = ESkillCastResult::Fail_InExecution;
    auto errorText = LOCTEXT("SkillCastResult_InExecutionText", "Couldn't cast skill, it's already in execution");
    return FSkillCastResult(result, MoveTemp(errorText));
}

FORCEINLINE FSkillCastResult FSkillCastResult::CastFail_InsufficientMana() {
    constexpr auto result = ESkillCastResult::Fail_InsufficientMana;
    auto errorText = LOCTEXT("SkillCastResult_InsufficientManaText", "Couldn't cast skill, insufficient mana.");
    return FSkillCastResult(result, MoveTemp(errorText));
}

FORCEINLINE FSkillCastResult FSkillCastResult::CastDeferred() {
    return FSkillCastResult{ESkillCastResult::Deferred, FText{}};
}

FORCEINLINE FSkillCastResult FSkillCastResult::CastSuccess_IntoChanneling() {
    return FSkillCastResult{ESkillCastResult::Success_IntoChanneling, FText{}};
}

FORCEINLINE FSkillCastResult FSkillCastResult::CastSuccess_IntoExecutionEnd() {
    return FSkillCastResult{ESkillCastResult::Success_IntoExecutionEnd, FText{}};
}

#undef LOCTEXT_NAMESPACE
