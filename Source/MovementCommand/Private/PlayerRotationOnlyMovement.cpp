// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRotationOnlyMovement.h"

void UPlayerRotationOnlyMovement::OnMovementTick(TObjectPtr<APlayerController> const& playerController, float deltaTime) {
    /* Needed to interrupt any ongoing pathfinding-like movement (e.g. roto-translation) immediately. */
    if (playerController->IsFollowingAPath()) {
        playerController->StopMovement();
    }

    FVector cursorLoc{};
    FVector cursorDir{};
    playerController->DeprojectMousePositionToWorld(cursorLoc, cursorDir);

    // Computing the plane with normal (0, 0, 1) containing the pawn's location in world coordinates
    auto const pawn = playerController->GetPawn();
    auto const pawnZ = pawn->GetActorLocation().Z;
    FPlane const pawnPlane{0., 0., 1., pawnZ};

    // We project the cursor location on the aforementioned plane, then express it in the pawn's reference frame to
    // extract the yaw rotation we need for the pawn to look at the cursor.
    auto const wrCursorLocOnPawnPlane = FMath::LinePlaneIntersection(cursorLoc, cursorLoc + cursorDir * LINE_LENGTH, pawnPlane);
    auto const prCursorLocOnPawnPlane = pawn->GetTransform().InverseTransformPosition(wrCursorLocOnPawnPlane);

    auto const angleBetweenPawnAndCursor = prCursorLocOnPawnPlane.Rotation().Yaw;
    double const maxAngleThisFrame = _angularVelocityInDegrees * deltaTime;
    auto clampedAngle = FMath::Clamp(FMath::Abs(angleBetweenPawnAndCursor), 0.0, maxAngleThisFrame);
    if (angleBetweenPawnAndCursor < 0.0) {
        clampedAngle *= -1.0;
    }

    pawn->AddActorLocalRotation(FRotator{0.0, clampedAngle, 0.0});
}
