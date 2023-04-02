// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CastEntityInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCastEntity : public UInterface {
    GENERATED_BODY()
};

/* Interface representing some entity spawned by a spell caster using some skill. */
class ICastEntity {
    GENERATED_BODY()

public:
    virtual void SetLifetime(double lifetime) {}
    virtual void SetCaster(TWeakObjectPtr<AActor> caster) {}
};
