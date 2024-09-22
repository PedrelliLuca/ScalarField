// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementCommand.h"

#include "PlayerRotationOnlyMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UPlayerRotationOnlyMovement : public UPlayerMovementCommand {
    GENERATED_BODY()

public:
    void OnSetDestination(TObjectPtr<APlayerController> const& playerController, FVector const& destination, FPlayerInputData const& inputData) override {}
    void OnStopMovement(TObjectPtr<APlayerController> const& playerController) override {}
    void OnMovementTick(TObjectPtr<APlayerController> const& playerController, float deltaTime) override;

private:
    UPROPERTY(EditAnywhere)
    float _angularVelocityInDegrees = 180.0f;

    static constexpr int32 LINE_LENGTH = 10000;
};
