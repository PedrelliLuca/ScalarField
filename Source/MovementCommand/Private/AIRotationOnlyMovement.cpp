// Fill out your copyright notice in the Description page of Project Settings.

#include "AIRotationOnlyMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void UAIRotationOnlyMovement::OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) {
	if (aiController->IsFollowingAPath()) {
		aiController->StopMovement();
	}

	const auto aiPawn = aiController->GetPawn();

	auto wrDestination = destination;
	wrDestination.Z = aiPawn->GetActorLocation().Z;
	const auto prDestination = aiPawn->GetTransform().InverseTransformPosition(wrDestination);
	const auto prRotation = prDestination.Rotation();
	_absDegreesToRotate = FMath::Abs(prRotation.Yaw);
	_rotationSign = FMath::Sign(prRotation.Yaw);
	_degreesSoFar = 0.0;
}

void UAIRotationOnlyMovement::OnStopMovement(const TObjectPtr<AAIController>& aiController) {
	aiController->StopMovement();
}

void UAIRotationOnlyMovement::OnMovementTick(const TObjectPtr<AAIController>& aiController, const float deltaTime) {
	/* Needed to interrupt any ongoing pathfinding-like movement (e.g. roto-translation) immediately. */
	if (aiController->IsFollowingAPath()) {
		aiController->StopMovement();
	}

	if (FMath::IsNearlyEqual(_absDegreesToRotate, _degreesSoFar)) {
		return;
	}

	auto degreesThisTick = _degreesPerSecond * deltaTime;
	if (_degreesSoFar + degreesThisTick > _absDegreesToRotate) {
		degreesThisTick = _absDegreesToRotate - _degreesSoFar;
	}
	
	aiController->GetPawn()->AddActorLocalRotation(_rotationSign * FRotator{0.0, degreesThisTick, 0.0});

	_degreesSoFar += degreesThisTick;
}
