// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "ScalarFieldCharacter.h"

AScalarFieldPlayerController::AScalarFieldPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AScalarFieldPlayerController::PlayerTick(float deltaTime) {
	Super::PlayerTick(deltaTime);

	if(_bInputPressed) {
		_followTime += deltaTime;

		// Look for the touch location
		FVector hitLocation = FVector::ZeroVector;
		FHitResult hit;
		if(_bIsTouch) {
			GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility, true, hit);
		}
		else {
			GetHitResultUnderCursor(ECC_Visibility, true, hit);
		}
		hitLocation = hit.Location;

		// Direct the Pawn towards that location
		APawn* const myPawn = GetPawn();
		if(myPawn) {
			FVector worldDirection = (hitLocation - myPawn->GetActorLocation()).GetSafeNormal();
			myPawn->AddMovementInput(worldDirection, 1.f, false);
		}
	}
	else {
		_followTime = 0.f;
	}
}

void AScalarFieldPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AScalarFieldPlayerController::_onSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AScalarFieldPlayerController::_onSetDestinationReleased);

	InputComponent->BindAction("Skill1Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill1Cast);
	InputComponent->BindAction("Skill2Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill2Cast);
	InputComponent->BindAction("Skill3Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill3Cast);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AScalarFieldPlayerController::_onTouchPressed);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &AScalarFieldPlayerController::_onTouchReleased);

}

void AScalarFieldPlayerController::_onSetDestinationPressed() {
	// We flag that the input is being pressed
	_bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void AScalarFieldPlayerController::_onSetDestinationReleased() {
	// Player is no longer pressing the input
	_bInputPressed = false;

	// If it was a short press
	if(_followTime <= _shortPressThreshold) {
		// We look for the location in the world where the player has pressed the input
		FVector hitLocation = FVector::ZeroVector;
		FHitResult hit;
		GetHitResultUnderCursor(ECC_Visibility, true, hit);
		hitLocation = hit.Location;

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, hitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, _fxCursor, hitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void AScalarFieldPlayerController::_onTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location) {
	_bIsTouch = true;
	_onSetDestinationPressed();
}

void AScalarFieldPlayerController::_onTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location) {
	_bIsTouch = false;
	_onSetDestinationReleased();
}

void AScalarFieldPlayerController::_onSkill1Cast() {
	if (const auto sfCharacter = Cast<AScalarFieldCharacter>(GetPawn())) {
		sfCharacter->CastSkillAtIndex(1);
	}
}

void AScalarFieldPlayerController::_onSkill2Cast() {
	if (const auto sfCharacter = Cast<AScalarFieldCharacter>(GetPawn())) {
		sfCharacter->CastSkillAtIndex(2);
	}
}

void AScalarFieldPlayerController::_onSkill3Cast() {
	if (const auto sfCharacter = Cast<AScalarFieldCharacter>(GetPawn())) {
		sfCharacter->CastSkillAtIndex(3);
	}
}
