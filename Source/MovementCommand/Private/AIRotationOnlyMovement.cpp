// Fill out your copyright notice in the Description page of Project Settings.

#include "AIRotationOnlyMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void UAIRotationOnlyMovement::OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) {
	const auto aiPawn = aiController->GetPawn();

	// World reference (wr) frame
	const auto wrDestination = destination;

	// Pawn reference (pr) frame
	const auto prDestination = aiPawn->GetTransform().InverseTransformPosition(wrDestination);
	const auto prRotation = prDestination.Rotation();

	_absDegreesToRotate = FMath::Abs(prRotation.Yaw);
	_rotationSign = FMath::Sign(prRotation.Yaw);
	_degreesSoFar = 0.0;

	_setIsMoving(true);
}

void UAIRotationOnlyMovement::OnStopMovement(const TObjectPtr<AAIController>& aiController) {
	aiController->StopMovement();
	_absDegreesToRotate = 0.0;
	_degreesSoFar = 0.0;
	_setIsMoving(false);
}

void UAIRotationOnlyMovement::OnMovementTick(const TObjectPtr<AAIController>& aiController, const float deltaTime) {
	if (FMath::IsNearlyEqual(_absDegreesToRotate, _degreesSoFar)) {
		if (IsMoving()) {
			_setIsMoving(false);
		}
		return;
	}

	auto degreesThisTick = _degreesPerSecond * deltaTime;
	if (_degreesSoFar + degreesThisTick > _absDegreesToRotate) {
		degreesThisTick = _absDegreesToRotate - _degreesSoFar;
	}
	
	aiController->GetPawn()->AddActorLocalRotation(_rotationSign * FRotator{0.0, degreesThisTick, 0.0});

	_degreesSoFar += degreesThisTick;
}
