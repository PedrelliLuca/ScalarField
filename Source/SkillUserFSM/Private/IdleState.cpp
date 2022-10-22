// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"

TObjectPtr<USkillUserState> UIdleState::OnLMBPress(TObjectPtr<AController> controller) {
	// We flag that the input is being pressed
	_bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	controller->StopMovement();

	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UIdleState::OnLMBRelease(TObjectPtr<APlayerController> controller) {
	// Player is no longer pressing the input
	_bInputPressed = false;

	// If it was a short press
	if (_followTime <= _shortPressThreshold) {
		// We look for the location in the world where the player has pressed the input
		FVector hitLocation = FVector::ZeroVector;
		FHitResult hit;
		controller->GetHitResultUnderCursor(ECC_Visibility, true, hit);
		hitLocation = hit.Location;

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, hitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(controller, _fxCursor, hitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UIdleState::OnKey1Press(TObjectPtr<AController> controller) {
	return TObjectPtr<USkillUserState>();
}

void UIdleState::Tick(float deltaTime, TObjectPtr<APlayerController> controller) {
	if (_bInputPressed) {
		_followTime += deltaTime;

		// Look for the touch location
		FVector hitLocation = FVector::ZeroVector;
		FHitResult hit;
		controller->GetHitResultUnderCursor(ECC_Visibility, true, hit);
		hitLocation = hit.Location;

		// Direct the Pawn towards that location
		APawn* const myPawn = controller->GetPawn();
		if (myPawn) {
			FVector worldDirection = (hitLocation - myPawn->GetActorLocation()).GetSafeNormal();
			myPawn->AddMovementInput(worldDirection, 1.f, false);
		}
	}
	else {
		_followTime = 0.f;
	}
}

void UIdleState::OnEnter(TObjectPtr<AController> controller) {
}

void UIdleState::OnLeave(TObjectPtr<AController> controller) {
}
