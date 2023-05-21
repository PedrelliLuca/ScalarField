// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillTargetingCondition.h"

#include "DerivesFromTargetingCondition.generated.h"

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SKILLTARGETING_API UDerivesFromTargetingCondition : public USkillTargetingCondition {
    GENERATED_BODY()

public:
    bool IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const override;

    void SetSkillCaster(TObjectPtr<AActor> skillCaster) override {}

private:
    /** \brief Determines whether we want targets that are ignitable or targets that aren't. */
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> _classToDeriveFrom = AActor::StaticClass();
};
