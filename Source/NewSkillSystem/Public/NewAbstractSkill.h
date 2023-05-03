// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCastResult.h"

#include "NewAbstractSkill.generated.h"

/**
 *
 */
UCLASS(NotBlueprintable, Abstract, DefaultToInstanced)
class NEWSKILLSYSTEM_API UNewAbstractSkill : public UObject {
    GENERATED_BODY()

public:
    FSkillCastResult TryCast();

    void SetCaster(TObjectPtr<AActor> caster);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Abstract Skill", meta = (ClampMin = "0"))
    float _cooldown = 1.0f;

private:
    void _onCooldownEnded();

    TWeakObjectPtr<AActor> _caster;

    FTimerHandle _cooldownTimer{};
    bool _onCooldown = false;
};
