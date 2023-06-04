// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "SkillTargetingCondition.h"

#include "HasComponentsTargetingCondition.generated.h"

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SKILLTARGETING_API UHasComponentsTargetingCondition : public USkillTargetingCondition {
    GENERATED_BODY()

public:
    bool IsVerifiedForTarget(ISkillTarget* skillTarget) const override;

    void SetSkillCaster(TObjectPtr<AActor> skillCaster) override {}

protected:
    /** \brief The components we want the target to have. */
    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<UActorComponent>> _requiredComponents{};
};
