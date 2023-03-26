// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "Parameters/MovementParameters.h"

#include "AIMovementCommand.generated.h"

// Broadcasts the new value of the _isMoving boolean when it changes
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementStateChanged, bool);

UCLASS(NotBlueprintable, Abstract)
class UAIMovementCommand : public UObject {
    GENERATED_BODY()

public:
    virtual void OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination)
        PURE_VIRTUAL(UAIMovementCommand::OnSetDestination, return;);
    virtual void OnStopMovement(const TObjectPtr<AAIController>& aiController) PURE_VIRTUAL(UAIMovementCommand::OnStopMovement, return;);
    virtual void OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) PURE_VIRTUAL(UAIMovementCommand::OnMovementTick, return;);

    virtual void SetMovementParameters(const FMovementParameters& params) PURE_VIRTUAL(UAIMovementCommand::SetMovementParameters, return;);

    bool IsMoving() const { return _isMoving; }

    /** \brief Needed to warn the Behavior Tree that we're (not) moving. This is needed because, by design, movement nodes in ScalarField do not stop the BT
     * from flowing */
    FOnMovementStateChanged& OnMovementStateChanged() { return _onMovementStateChanged; }

protected:
    void _setIsMoving(bool isMoving) {
        // We want to broadcast only if the value is actually changing, not every time we call this.
        if (isMoving != _isMoving) {
            _isMoving = isMoving;
            _onMovementStateChanged.Broadcast(_isMoving);
        }
    }

private:
    FOnMovementStateChanged _onMovementStateChanged;

    bool _isMoving = false;
};
