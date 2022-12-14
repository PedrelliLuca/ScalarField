// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRotationOnlyMovement.h"

void UPlayerRotationOnlyMovement::OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) {
	if (playerController->IsFollowingAPath()) {
		playerController->StopMovement();
	}

	FVector cursorLoc{};
	FVector cursorDir{};
	playerController->DeprojectMousePositionToWorld(cursorLoc, cursorDir);

	// Computing the plane with normal (0, 0, 1) containing the pawn's location in world coordinates
	const auto pawn = playerController->GetPawn();
	const auto pawnZ = pawn->GetActorLocation().Z;
	const FPlane pawnPlane{0., 0., 1., pawnZ};

	// We project the cursor location on the aforementioned plane, then express it in the pawn's reference frame to
	// extract the yaw rotation we need for the pawn to look at the cursor.
	const auto wrCursorLocOnPawnPlane = FMath::LinePlaneIntersection(cursorLoc, cursorLoc + cursorDir * LINE_LENGTH, pawnPlane);
	const auto prCursorLocOnPawnPlane = pawn->GetTransform().InverseTransformPosition(wrCursorLocOnPawnPlane);
	pawn->AddActorLocalRotation(prCursorLocOnPawnPlane.Rotation());
}
