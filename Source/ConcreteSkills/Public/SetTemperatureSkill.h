// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NewAbstractSkill.h"

#include "SetTemperatureSkill.generated.h"

/**
 * \brief A cast-only skill that spawns an actor attached to the target. The attachment lasts for the entire channeling phase.
 */
UCLASS(Blueprintable)
class CONCRETESKILLS_API USetTemperatureSkill : public UNewAbstractSkill {
    GENERATED_BODY()

private:
    void _skillCast() override;
    void _skillChannelingTick(float deltaTime) override;
    void _skillAbort() override;

    UPROPERTY(EditDefaultsOnly)
    float _temperatureToSet = 300.0f;
};
