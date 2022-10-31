// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRotationOnlyMovement.h"

void UPlayerRotationOnlyMovement::OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) {
	FHitResult hit;
	playerController->GetHitResultUnderCursor(ECC_Visibility, true, hit);
	const FVector worldCursorLoc = hit.Location;

	const auto pawn = playerController->GetPawn();
	const FVector pawnCursorLoc = pawn->GetTransform().InverseTransformPosition(worldCursorLoc);

	// The plane with normal (0, 0, 1) containing the pawn's location, which is just (0, 0, 0) in its reference frame
	const FPlane pawnPlane{0., 0., 1., 0.};
	const auto projectedPawnCursorLoc = FPlane::PointPlaneProject(pawnCursorLoc, pawnPlane);

	pawn->AddActorLocalRotation(projectedPawnCursorLoc.Rotation());
}
