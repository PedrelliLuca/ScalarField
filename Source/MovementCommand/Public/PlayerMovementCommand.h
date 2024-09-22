// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInputData.h"

#include "PlayerMovementCommand.generated.h"

UCLASS(NotBlueprintable, Abstract)
class UPlayerMovementCommand : public UObject {
    GENERATED_BODY()

public:
    virtual void OnSetDestination(TObjectPtr<APlayerController> const& playerController, FVector const& destination, FPlayerInputData const& inputData)
        PURE_VIRTUAL(UPlayerMovementCommand::OnSetDestination, return;);
    virtual void OnStopMovement(TObjectPtr<APlayerController> const& playerController) PURE_VIRTUAL(UPlayerMovementCommand::OnStopMovement, return;);
    virtual void OnMovementTick(TObjectPtr<APlayerController> const& playerController, float deltaTime)
        PURE_VIRTUAL(UPlayerMovementCommand::OnMovementTick, return;);
};
