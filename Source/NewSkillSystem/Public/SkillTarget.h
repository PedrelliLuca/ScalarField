// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillTarget.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class USkillTarget : public UInterface {
    GENERATED_BODY()
};

/* Interface representing something that can be employed as target for a skill. */
class ISkillTarget {
    GENERATED_BODY()

public:
    /** \brief Returns the location of the target in the world. */
    virtual FVector GetTargetLocation() const = 0;
};
