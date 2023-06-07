// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIMovementCommand.h"
#include "Parameters/RotationOnlyMovementParameters.h"

#include "AIRotationOnlyMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UAIRotationOnlyMovement : public UAIMovementCommand {
    GENERATED_BODY()

public:
    void OnSetDestination(const FVector& destination) final;
    void OnStopMovement() final;
    void OnMovementTick(float deltaTime) final;

    void SetMovementParameters(const FMovementParameters& params) final;

    bool IsMoving() const final;

private:
    UPROPERTY(EditDefaultsOnly)
    FRotationOnlyMovementParameters _movementParameters;

    double _absDegreesToRotate = 0.0;
    double _rotationSign = 1.0;

    double _degreesSoFar = 0.0;

    bool _isRotating = false;
};
