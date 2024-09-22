// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "Parameters/MovementParameters.h"

#include "AIMovementCommand.generated.h"

UCLASS(NotBlueprintable, Abstract)
class UAIMovementCommand : public UObject {
    GENERATED_BODY()

public:
    virtual void OnSetDestination(FVector const& destination) PURE_VIRTUAL(UAIMovementCommand::OnSetDestination, return;);
    virtual void OnStopMovement() PURE_VIRTUAL(UAIMovementCommand::OnStopMovement, return;);
    virtual void OnMovementTick(float deltaTime) PURE_VIRTUAL(UAIMovementCommand::OnMovementTick, return;);

    virtual void SetMovementParameters(FMovementParameters const& params) PURE_VIRTUAL(UAIMovementCommand::SetMovementParameters, return;);

    virtual bool IsMoving() const PURE_VIRTUAL(UAIMovementCommand::IsMoving, return false;);

    void SetAIController(TObjectPtr<AAIController> const& aiController) {
        if (ensureMsgf(!_aiController.IsValid(), TEXT("AI Controller can only be set once and has already been set"))) {
            check(IsValid(aiController));
            _aiController = aiController;
        }
    }

protected:
    TWeakObjectPtr<AAIController> _aiController = nullptr;
};
