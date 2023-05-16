// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillTargetingCondition.h"

#include "IgnitableTargetingCondition.generated.h"

/** \brief Ways to compare the distance between the caster and the target. E.g., "Less Than" means that the condition is verified if the distance between the
 * two is less than this condition's distance value. */
UENUM(BlueprintType)
enum class EFlammability : uint8
{
    F_None UMETA(DisplayName = "None"),
    F_Ignitable UMETA(DisplayName = "Ignitable"),
    F_NotIgnitable UMETA(DisplayName = "Not Ignitable"),
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SKILLTARGETING_API UIgnitableTargetingCondition : public USkillTargetingCondition {
    GENERATED_BODY()

public:
    bool IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const override;

    void SetSkillCaster(TObjectPtr<AActor> skillCaster) override {}

protected:
    /** \brief Determines whether we want targets that are ignitable or targets that aren't. */
    UPROPERTY(EditAnywhere)
    EFlammability _flammability = EFlammability::F_None;
};
