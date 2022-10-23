// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "NiagaraFunctionLibrary.h"
#include "ScalarFieldCharacter.h"

AScalarFieldPlayerController::AScalarFieldPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AScalarFieldPlayerController::PlayerTick(const float deltaTime) {
	Super::PlayerTick(deltaTime);

	const auto newState = _state->OnTick(deltaTime, this);
	_changingStateRoutine(newState);

	if (_state->DisablesMovement()) {
		return;
	}

	if (_bInputPressed) {
		_followTime += deltaTime;

		// Look for the touch location
		FVector hitLocation = FVector::ZeroVector;
		FHitResult hit;
		GetHitResultUnderCursor(ECC_Visibility, true, hit);
		hitLocation = hit.Location;

		// Direct the Pawn towards that location
		APawn* const myPawn = GetPawn();
		if (myPawn) {
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

	InputComponent->BindAction("SetTarget", IE_Released, this, &AScalarFieldPlayerController::_onSetTargetPressed);

	InputComponent->BindAction("Skill1Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill1Cast);
	InputComponent->BindAction("Skill2Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill2Cast);
	InputComponent->BindAction("Skill3Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill3Cast);
	InputComponent->BindAction("AbortCast", IE_Pressed, this, &AScalarFieldPlayerController::_onCastAborted);
}

void AScalarFieldPlayerController::BeginPlay() {
	Super::BeginPlay();
	_state = NewObject<UIdleState>(this, UIdleState::StaticClass());
}

void AScalarFieldPlayerController::_onSetDestinationPressed() {
	if (_state->DisablesMovement()) {
		return;
	}

	// We flag that the input is being pressed
	_bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void AScalarFieldPlayerController::_onSetDestinationReleased() {
	if (_state->DisablesMovement()) {
		return;
	}

	// Player is no longer pressing the input
	_bInputPressed = false;

	// If it was a short press
	if (_followTime <= _shortPressThreshold) {
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

void AScalarFieldPlayerController::_onSetTargetPressed() {
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, true, hit);
	const auto newState = _state->OnTargeting(hit.GetActor(), this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onSkill1Cast() {
	const auto newState = _state->OnBeginSkillExecution(1, this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onSkill2Cast() {
	const auto newState = _state->OnBeginSkillExecution(2, this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onSkill3Cast() {
	const auto newState = _state->OnBeginSkillExecution(3, this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onCastAborted() {
	const auto newState = _state->OnSkillExecutionAborted(this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_changingStateRoutine(TObjectPtr<USkillUserState> newState) {
	if (IsValid(newState)) {
		_state->OnLeave(this);
		_state = newState;
		_state->OnEnter(this);
	}
}
