// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIMovementCommand.h"
#include "Parameters/RotoTranslationMovementParameters.h"

#include "AIRotoTranslationMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UAIRotoTranslationMovement : public UAIMovementCommand {
    GENERATED_BODY()

public:
    void OnSetDestination(const FVector& destination) final;
    void OnStopMovement() final;
    void OnMovementTick(float deltaTime) final;

    void SetMovementParameters(const FMovementParameters& params) final;

    bool IsMoving() const final;

private:
    UPROPERTY(EditDefaultsOnly)
    FRotoTranslationMovementParameters _movementParameters;
};
