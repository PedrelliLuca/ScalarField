// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementCommand.h"

#include "PlayerStillMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UPlayerStillMovement : public UPlayerMovementCommand {
    GENERATED_BODY()

public:
    void OnSetDestination(const TObjectPtr<APlayerController>& playerController) final {}
    void OnStopMovement(const TObjectPtr<APlayerController>& playerController) final {}
    void OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) final {
        if (playerController->IsFollowingAPath()) {
            playerController->StopMovement();
        }
    }
};
