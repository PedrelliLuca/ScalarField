// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Represents the possible outcomes of a skill execution request.
enum class ESkillChannelingResult : uint8
{
    None,
    Fail_InsufficientMana,
    Success
};

// Struct that collects the enum representing the channeling phase outcome and the error associated with it
// TODO: The error is just a message for the time being, but in the future a more refined error management will be needed.
class FSkillChannelingResult {
public:
    // Functions to standardize instances of this class
    static FSkillChannelingResult ChannelingFail_InsufficientMana();
    static FSkillChannelingResult ChannelingSuccess();

    ESkillChannelingResult GetChannelingResult() const { return _result; }

    const FText& GetErrorText() const { return _errorText; }

private:
    FSkillChannelingResult(ESkillChannelingResult result, FText&& errorText)
        : _result(result)
        , _errorText(MoveTemp(errorText)) {}

    ESkillChannelingResult _result = ESkillChannelingResult::None;

    FText _errorText{};
};

#define LOCTEXT_NAMESPACE "SkillChannelingResult"

// TODO: in the future take skill name as string parameter and use it to build better error text
FORCEINLINE FSkillChannelingResult FSkillChannelingResult::ChannelingFail_InsufficientMana() {
    constexpr auto result = ESkillChannelingResult::Fail_InsufficientMana;
    auto errorText = LOCTEXT("SkillChanneling_InsufficientManaText", "Skill channeling interrupted, insufficient mana.");
    return FSkillChannelingResult(result, MoveTemp(errorText));
}

FORCEINLINE FSkillChannelingResult FSkillChannelingResult::ChannelingSuccess() {
    return FSkillChannelingResult{ESkillChannelingResult::Success, FText{}};
}

#undef LOCTEXT_NAMESPACE