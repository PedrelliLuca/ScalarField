// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RotationOnlyMovementParameters.generated.h"

USTRUCT(Blueprintable)
struct MOVEMENTCOMMAND_API FRotationOnlyMovementParameters {
    GENERATED_BODY()

public:
    /** \brief Angular velocity of the pawn in degrees per second. */
    UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f))
    float DegreesPerSecond = 60.0f;

    /** \brief How far from the target angle we consider the rotation to be complete. */
    UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f))
    float AngularTolerance = 5.0f;
};
