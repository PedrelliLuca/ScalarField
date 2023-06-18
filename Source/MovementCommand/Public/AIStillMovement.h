// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIMovementCommand.h"
#include "CoreMinimal.h"

#include "AIStillMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UAIStillMovement : public UAIMovementCommand {
    GENERATED_BODY()

public:
    void OnSetDestination(const FVector& destination) final { _aiController->StopMovement(); }

    void OnStopMovement() final { _aiController->StopMovement(); }

    void OnMovementTick(float deltaTime) final {}

    void SetMovementParameters(const FMovementParameters& params) final {}

    bool IsMoving() const final { return false; }
};
