// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillSpawnedEntityInterface.generated.h"

UINTERFACE(MinimalAPI)
class USkillSpawnedEntity : public UInterface {
    GENERATED_BODY()
};

/* Interface representing an inventory. */
class ISkillSpawnedEntity {
    GENERATED_BODY()

public:
    virtual void SetLifetime(double lifetime) {}
    virtual void SetCaster(TWeakObjectPtr<AActor> caster) {}
};
