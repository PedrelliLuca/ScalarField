// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "GameFramework/SpringArmComponent.h"

#include "CastAttachedEntitySkill.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UCastAttachedEntitySkill : public UAbstractSkill {
    GENERATED_BODY()

public:
    void ExecuteCast(TObjectPtr<AActor> caster) override;
    void Abort() override;

private:
    void _cleanupCallback();

    FTimerHandle _timerHandle;

    TWeakObjectPtr<USpringArmComponent> _spawnSpringArm = nullptr;
    TWeakObjectPtr<AActor> _spawnActor = nullptr;
};
