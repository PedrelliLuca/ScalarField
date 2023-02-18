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
	aiPawn->AddActorLocalRotation(prDestination.Rotation());
}

void UAIRotationOnlyMovement::OnStopMovement(const TObjectPtr<AAIController>& aiController) {
	aiController->StopMovement();
}

void UAIRotationOnlyMovement::OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) {
	// Do we even need this for AI?
}
