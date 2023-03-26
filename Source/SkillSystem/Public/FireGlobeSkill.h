// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractSkill.h"
#include "CoreMinimal.h"

#include "FireGlobeSkill.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UFireGlobeSkill : public UAbstractSkill {
    GENERATED_BODY()

public:
    void ExecuteCast(TObjectPtr<AActor> caster) override;

    bool IsValidTarget(int32 targetIndex, TObjectPtr<AActor> target) const override;

#if DO_CHECK
    void CheckParametersSanity() const override;
#endif

private:
    bool _isControlled(TObjectPtr<AActor> target) const;
};
