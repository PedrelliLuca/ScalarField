// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Represents the possible outcomes of a skill execution request.
enum class ESkillCastResult : uint8
{
    None,
    Fail_OnCooldown,
    Fail_MissingTarget,
    Success
};

// Struct that collects the enum representing the cast request outcome and the error associated with it
// TODO: The error is just a message for the time being, but in the future a more refined error management will be needed.
class FSkillCastResult {
public:
    // Functions to standardize instances of this class
    static FSkillCastResult CastFail_Cooldown();
    static FSkillCastResult CastFail_MissingTarget();
    static FSkillCastResult CastSuccess();

    ESkillCastResult GetCastResult() const { return _result; }

    const FText& GetErrorText() const { return _errorText; }

private:
    FSkillCastResult(ESkillCastResult result, FText&& errorText)
        : _result(result)
        , _errorText(MoveTemp(errorText)) {}

    ESkillCastResult _result = ESkillCastResult::None;

    FText _errorText{};
};

#define LOCTEXT_NAMESPACE "SkillExecutionResult"

FORCEINLINE FSkillCastResult FSkillCastResult::CastFail_Cooldown() {
    auto result = ESkillCastResult::Fail_OnCooldown;
    auto errorText = LOCTEXT("SkillCastOnCooldownText", "Couldn't cast skill, it's on cooldown.");

    return FSkillCastResult{result, MoveTemp(errorText)};
}

// TODO: in the future, this could take an integer representing the number of missing targets, so that the message could be more meaningful.
FORCEINLINE FSkillCastResult FSkillCastResult::CastFail_MissingTarget() {
    auto result = ESkillCastResult::Fail_OnCooldown;
    auto errorText = LOCTEXT("SkillCastMissingTargetText", "Couldn't cast skill, targets are missing");

    return FSkillCastResult{result, MoveTemp(errorText)};
}

FORCEINLINE FSkillCastResult FSkillCastResult::CastSuccess() {
    return FSkillCastResult{ESkillCastResult::Success, FText{}};
}

#undef LOCTEXT_NAMESPACE
