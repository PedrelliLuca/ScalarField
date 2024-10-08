// Fill out your copyright notice in the Description page of Project Settings.

#include "AIRotationOnlyMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void UAIRotationOnlyMovement::OnSetDestination(FVector const& destination) {
    check(_aiController.IsValid());
    auto const aiPawn = _aiController->GetPawn();

    // World reference (wr) frame
    auto const wrDestination = destination;

    // Pawn reference (pr) frame
    auto const prDestination = aiPawn->GetTransform().InverseTransformPosition(wrDestination);
    auto const prRotation = prDestination.Rotation();

    _absDegreesToRotate = FMath::Abs(prRotation.Yaw);
    _rotationSign = FMath::Sign(prRotation.Yaw);
    _degreesSoFar = 0.0;
}

void UAIRotationOnlyMovement::OnStopMovement() {
    check(_aiController.IsValid());

    _aiController->StopMovement();
    _absDegreesToRotate = 0.0;
    _degreesSoFar = 0.0;
}

void UAIRotationOnlyMovement::OnMovementTick(float const deltaTime) {
    check(_aiController.IsValid());

    if (_degreesSoFar > _absDegreesToRotate - _movementParameters.AngularTolerance) {
        _isRotating = false;
        return;
    }

    _isRotating = true;

    auto degreesThisTick = _movementParameters.DegreesPerSecond * deltaTime;

    // Do not overshoot!
    if (_degreesSoFar + degreesThisTick > _absDegreesToRotate) {
        degreesThisTick = _absDegreesToRotate - _degreesSoFar;
    }

    _aiController->GetPawn()->AddActorLocalRotation(_rotationSign * FRotator{0.0, degreesThisTick, 0.0});

    _degreesSoFar += degreesThisTick;
}

void UAIRotationOnlyMovement::SetMovementParameters(FMovementParameters const& params) {
    _movementParameters = params.RotationOnlyMovementParameters;
}

bool UAIRotationOnlyMovement::IsMoving() const {
    return _isRotating;
}
