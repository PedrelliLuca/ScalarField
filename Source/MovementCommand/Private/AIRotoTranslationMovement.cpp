// Fill out your copyright notice in the Description page of Project Settings.

#include "AIRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void UAIRotoTranslationMovement::OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) {
	if (!aiController->IsFollowingAPath()) {
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiController, destination);
		_setIsMoving(true);
	}
}

void UAIRotoTranslationMovement::OnStopMovement(const TObjectPtr<AAIController>& aiController) {
	aiController->StopMovement();
	_setIsMoving(false);
}

void UAIRotoTranslationMovement::OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) {
	// Do we even need this for AI?
}
