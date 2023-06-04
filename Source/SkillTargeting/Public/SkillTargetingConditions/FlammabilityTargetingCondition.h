// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillTargetingCondition.h"

#include "FlammabilityTargetingCondition.generated.h"

/** \brief Ways to check for flammability. "Ignitable" means that the condition is verified if the target can be ignited, "Not Ignitable" is the target is
 * fire-proof. */
UENUM(BlueprintType)
enum class EFlammability : uint8
{
    F_None UMETA(DisplayName = "None"),
    F_Ignitable UMETA(DisplayName = "Ignitable"),
    F_NotIgnitable UMETA(DisplayName = "Not Ignitable"),
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SKILLTARGETING_API UFlammabilityTargetingCondition : public USkillTargetingCondition {
    GENERATED_BODY()

public:
    bool IsVerifiedForTarget(ISkillTarget* skillTarget) const override;

    void SetSkillCaster(TObjectPtr<AActor> skillCaster) override {}

protected:
    /** \brief Determines whether we want targets that are ignitable or targets that aren't. */
    UPROPERTY(EditAnywhere)
    EFlammability _flammability = EFlammability::F_None;
};
