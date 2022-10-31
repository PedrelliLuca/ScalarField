// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"

void UPlayerRotoTranslationMovement::OnSetDestination(const TObjectPtr<APlayerController>& playerController) {
	// Player is no longer pressing the input
	_bInputPressed = false;

	// If it was a short press
	if (_followTime <= _shortPressThreshold) {
		// We look for the location in the world where the player has pressed the input
		FHitResult hit;
		playerController->GetHitResultUnderCursor(ECC_Visibility, true, hit);
		const FVector hitLocation = hit.Location;

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(playerController, hitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, _fxCursor, hitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void UPlayerRotoTranslationMovement::OnStopMovement(const TObjectPtr<APlayerController>& playerController) {
	// We flag that the input is being pressed
	_bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	playerController->StopMovement();
}

void UPlayerRotoTranslationMovement::OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) {
	if (!_bInputPressed) {
		_followTime = 0;
		return;
	}
	
	_followTime += deltaTime;

	// We look for the location in the world where the player has pressed the input
	FHitResult hit;
	playerController->GetHitResultUnderCursor(ECC_Visibility, true, hit);
	const FVector hitLocation = hit.Location;

	// Direct the Pawn towards that location
	if (const auto pawn = playerController->GetPawn()) {
		const FVector worldDirection = (hitLocation - pawn->GetActorLocation()).GetSafeNormal();
		pawn->AddMovementInput(worldDirection, 1.f, false);
	}
}
