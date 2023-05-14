// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillTargetingCondition.h"

#include "IgnitableTargetingCondition.generated.h"

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SKILLTARGETING_API UIgnitableTargetingCondition : public USkillTargetingCondition {
    GENERATED_BODY()

public:
    bool IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const override;

    void SetSkillCaster(TObjectPtr<AActor> skillCaster) override;
};
