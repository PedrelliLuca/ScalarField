// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillCastConditionInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class USkillCastCondition : public UInterface {
    GENERATED_BODY()
};

/* Interface representing an inventory. */
class ISkillCastCondition {
    GENERATED_BODY()

public:
    /** \brief Checks whether the cast condition is verified or not. */
    virtual bool IsVerified() const = 0;

    /** \brief Sets the subject, i.e. the AActor on which we'll check if the condition is verified or not. */
    virtual void SetConditionSubject(TObjectPtr<AActor> subject) = 0;
};
