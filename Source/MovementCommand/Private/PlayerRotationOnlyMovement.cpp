// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRotationOnlyMovement.h"

void UPlayerRotationOnlyMovement::OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) {
	FHitResult hit;
	playerController->GetHitResultUnderCursor(ECC_Visibility, true, hit);
	const FVector worldCursorLoc = hit.Location;

	const auto pawn = playerController->GetPawn();
	const FVector pawnCursorLoc = pawn->GetTransform().InverseTransformPosition(worldCursorLoc);

	pawn->AddActorLocalRotation(pawnCursorLoc.Rotation());
}
