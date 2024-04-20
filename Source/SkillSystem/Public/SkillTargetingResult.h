// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Represents the possible outcomes of a skill targeting request.
enum class ESkillTargetingResult : uint8
{
    None,
    Fail_InvalidTarget,
    Fail_AlreadyAvailableTargets,
    Success_KeepTargeting,
    Success_IntoCast
};

// Struct that collects the enum representing the targeting request outcome and the error associated with it
// TODO: The error is just a message for the time being, but in the future a more refined error management will be needed.
class FSkillTargetingResult {
public:
    // Functions to standardize instances of this class
    static FSkillTargetingResult TargetingFail_InvalidTarget();
    static FSkillTargetingResult TargetingFail_AlreadyAvailableTargets();
    static FSkillTargetingResult TargetingSuccess_KeepTargeting();
    static FSkillTargetingResult TargetingSuccess_IntoCast();

    ESkillTargetingResult GetTargetingResult() const { return _result; }

    const FText& GetErrorText() const { return _errorText; }

    bool IsFailure() const { return !_errorText.IsEmpty(); }

private:
    FSkillTargetingResult(ESkillTargetingResult result, FText&& errorText)
        : _result(result)
        , _errorText(MoveTemp(errorText)) {}

    ESkillTargetingResult _result = ESkillTargetingResult::None;

    FText _errorText{};
};

#define LOCTEXT_NAMESPACE "SkillTargetingResult"

FORCEINLINE FSkillTargetingResult FSkillTargetingResult::TargetingFail_InvalidTarget() {
    constexpr auto result = ESkillTargetingResult::Fail_InvalidTarget;
    auto errorText = LOCTEXT("SkillTargetingResult_InvalidTargetText", "Targeting failed, invalid target.");

    return FSkillTargetingResult{result, MoveTemp(errorText)};
}

FORCEINLINE FSkillTargetingResult FSkillTargetingResult::TargetingFail_AlreadyAvailableTargets() {
    constexpr auto result = ESkillTargetingResult::Fail_AlreadyAvailableTargets;
    auto errorText = LOCTEXT("SkillTargetingResult_AlreadyAvailableTargetsText", "Targeting failed, all targets are already avaiable.");

    return FSkillTargetingResult{result, MoveTemp(errorText)};
}

FORCEINLINE FSkillTargetingResult FSkillTargetingResult::TargetingSuccess_KeepTargeting() {
    return FSkillTargetingResult{ESkillTargetingResult::Success_KeepTargeting, FText{}};
}

FORCEINLINE FSkillTargetingResult FSkillTargetingResult::TargetingSuccess_IntoCast() {
    return FSkillTargetingResult{ESkillTargetingResult::Success_IntoCast, FText{}};
}

#undef LOCTEXT_NAMESPACE
