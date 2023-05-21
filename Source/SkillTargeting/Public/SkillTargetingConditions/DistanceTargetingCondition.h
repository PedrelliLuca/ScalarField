// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillTargetingCondition.h"

#include "DistanceTargetingCondition.generated.h"

/** \brief Ways to compare the distance between the caster and the target. E.g., "Less Than" means that the condition is verified if the distance between the
 * two is less than this condition's distance value. */
UENUM(BlueprintType)
enum class EDistanceComparisonStyle : uint8
{
    DCS_None UMETA(DisplayName = "None"),
    DCS_Equality UMETA(DisplayName = "Equality"),
    DCS_LessThan UMETA(DisplayName = "Less Than"),
    DCS_GreaterThan UMETA(DisplayName = "Greater Than")
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SKILLTARGETING_API UDistanceTargetingCondition : public USkillTargetingCondition {
    GENERATED_BODY()

public:
    bool IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const override;

    void SetSkillCaster(TObjectPtr<AActor> skillCaster) override;

protected:
    /** \brief The distance in cm for which we want to create a targeting condition. */
    UPROPERTY(EditAnywhere)
    float _targetingConditionDistance = 100.0f;

    /** \brief Determines how the target's distance will be compared with _targetingConditionDistance */
    UPROPERTY(EditAnywhere)
    EDistanceComparisonStyle _distanceComparison = EDistanceComparisonStyle::DCS_None;

private:
    TWeakObjectPtr<AActor> _caster = nullptr;
};
