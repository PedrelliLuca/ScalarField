// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillTarget.h"

#include "SkillTargetingCondition.generated.h"

/* Represents a condition that must be verified in order for a target to be valid for a skill. This can't be an interface because it's meant to be employed with
 * the "Instanced" UPROPERTY specifier, see UAbstractSkill. */
UCLASS(NotBlueprintable, Abstract)
class NEWSKILLSYSTEM_API USkillTargetingCondition : public UObject {
    GENERATED_BODY()

public:
    /** \brief Checks whether the targeting condition is verified or not for the given target. */
    virtual bool IsVerifiedForTarget(ISkillTarget* skillTarget) const PURE_VIRTUAL(USkillTargetingCondition::IsVerified, return false;);

    /** \brief The Skill Caster is the subject, i.e. the AActor on which we'll check if the condition is verified or not. */
    virtual void SetSkillCaster(TObjectPtr<AActor> skillCaster) PURE_VIRTUAL(USkillTargetingCondition::SetSkillCaster, return;);
};
