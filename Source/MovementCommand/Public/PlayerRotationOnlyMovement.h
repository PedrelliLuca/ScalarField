// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementCommand.h"

#include "PlayerRotationOnlyMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UPlayerRotationOnlyMovement : public UPlayerMovementCommand {
    GENERATED_BODY()

public:
    void OnSetDestination(const TObjectPtr<APlayerController>& playerController, const FVector& destination) override {}
    void OnStopMovement(const TObjectPtr<APlayerController>& playerController) override {}
    void OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) override;

private:
    UPROPERTY(EditAnywhere)
    float _angularVelocityInDegrees = 180.0f;  
    
    static constexpr int32 LINE_LENGTH = 10000;
};
