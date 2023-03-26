// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractSkill.h"
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"

#include "ConeOfColdSkill.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UConeOfColdSkill : public UAbstractSkill {
    GENERATED_BODY()

public:
    void ExecuteCast(TObjectPtr<AActor> caster) override;
    void ExecuteChannelingTick(float deltaTime, const TObjectPtr<AActor> caster) override;
    void Abort() override;

#if DO_CHECK
    void CheckParametersSanity() const override;
#endif

private:
    TWeakObjectPtr<USpringArmComponent> _spawnSpringArm = nullptr;
    TWeakObjectPtr<AActor> _spawnActor = nullptr;
    TWeakObjectPtr<UCapsuleComponent> _cone = nullptr;
};
