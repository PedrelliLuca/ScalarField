// Fill out your copyright notice in the Description page of Project Settings.

#include "AIRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"

void UAIRotoTranslationMovement::OnSetDestination(const FVector& destination) {
    check(_aiController.IsValid());

    if (_aiController->IsFollowingAPath() && !_aiController->GetPathFollowingComponent()->GetPathDestination().Equals(destination)) {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(_aiController.Get(), destination);
    }
}

void UAIRotoTranslationMovement::OnStopMovement() {
    check(_aiController.IsValid());
    _aiController->StopMovement();
}

void UAIRotoTranslationMovement::OnMovementTick(float deltaTime) {
}

void UAIRotoTranslationMovement::SetMovementParameters(const FMovementParameters& params) {
    _movementParameters = params.RotoTranslationMovementParameters;
}

bool UAIRotoTranslationMovement::IsMoving() const {
    return IsValid(_aiController->GetPathFollowingComponent()) && _aiController->IsFollowingAPath();
}
