// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillCastCondition.generated.h"

/* Represents a condition that must be verified in order to cast a skill. This can't be an interface because it's meant to be employed with the "Instanced"
 * UPROPERTY specifier, see UAbstractSkill. */
UCLASS(NotBlueprintable, Abstract)
class SKILLSYSTEM_API USkillCastCondition : public UObject {
    GENERATED_BODY()

public:
    /** \brief Checks whether the cast condition is verified or not. */
    virtual bool IsVerified() const PURE_VIRTUAL(USkillCastCondition::IsVerified, return false;);

    /** \brief Sets the subject, i.e. the AActor on which we'll check if the condition is verified or not. */
    virtual void SetConditionSubject(TObjectPtr<AActor> subject) PURE_VIRTUAL(USkillCastCondition::SetConditionSubject, return;);
};
