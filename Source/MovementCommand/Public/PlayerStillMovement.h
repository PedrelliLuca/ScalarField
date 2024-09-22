// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementCommand.h"

#include "PlayerStillMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UPlayerStillMovement : public UPlayerMovementCommand {
    GENERATED_BODY()

public:
    void OnSetDestination(TObjectPtr<APlayerController> const& playerController, FVector const& destination, FPlayerInputData const& inputData) final {}
    void OnStopMovement(TObjectPtr<APlayerController> const& playerController) final {}
    void OnMovementTick(TObjectPtr<APlayerController> const& playerController, float deltaTime) final {
        if (playerController->IsFollowingAPath()) {
            playerController->StopMovement();
        }
    }
};
