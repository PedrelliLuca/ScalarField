// Fill out your copyright notice in the Description page of Project Settings.

#include "AIRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void UAIRotoTranslationMovement::OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) {
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiController, destination);
}

void UAIRotoTranslationMovement::OnStopMovement(const TObjectPtr<AAIController>& aiController) {
	aiController->StopMovement();
}

void UAIRotoTranslationMovement::OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) {
	// Do we even need this for AI?
}
