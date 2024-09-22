// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillTarget.generated.h"

/** \brief Used to initialize ISkillTargets without knowing their concrete type. */
struct FSkillTargetPacket {
    TWeakObjectPtr<AActor> TargetActor = nullptr;
    TOptional<FVector> TargetCasterPlaneLocation{};
    // TODO: to be implemented in the future for skills that need a ground location, e.g. "Spawn Tree".
    // TOptional<FVector> TargetGroundLocation{};
};

UINTERFACE(MinimalAPI, NotBlueprintable)
class USkillTarget : public UInterface {
    GENERATED_BODY()
};

class ISkillTarget {
    GENERATED_BODY()

public:
    virtual void Init(FSkillTargetPacket const& targetPacket) = 0;

    virtual bool IsValidTarget() const = 0;

    /** \brief Returns the location of the target in the world. */
    virtual FVector GetTargetLocation() const = 0;
};
