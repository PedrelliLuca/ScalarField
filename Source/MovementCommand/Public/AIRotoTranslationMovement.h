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
    void OnSetDestination(FVector const& destination) final;
    void OnStopMovement() final;
    void OnMovementTick(float deltaTime) final;

    void SetMovementParameters(FMovementParameters const& params) final;

    bool IsMoving() const final;

private:
    UPathFollowingComponent* _initNavigationControl(AController& Controller);

    // NOTE: this function is a copy of UAIBlueprintHelperLibrary::SimpleMoveToLocation. Why not using the original then? Because I need to inject
    // the _movementParameters such as the AcceptanceRadius
    void _moveToLocation(AController* Controller, FVector const& GoalLocation);

    UPROPERTY(EditDefaultsOnly)
    FRotoTranslationMovementParameters _movementParameters;
};
