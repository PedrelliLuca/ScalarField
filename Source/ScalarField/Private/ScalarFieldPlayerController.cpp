// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldPlayerController.h"

#include "Engine/World.h"
#include "GameFramework/Pawn.h"

#include "ScalarFieldCharacter.h"

AScalarFieldPlayerController::AScalarFieldPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AScalarFieldPlayerController::PlayerTick(const float deltaTime) {
	Super::PlayerTick(deltaTime);
	_state->Tick(deltaTime, this);
}

void AScalarFieldPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AScalarFieldPlayerController::_onSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AScalarFieldPlayerController::_onSetDestinationReleased);

	InputComponent->BindAction("Skill1Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill1Cast);
	InputComponent->BindAction("Skill2Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill2Cast);
	InputComponent->BindAction("Skill3Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill3Cast);

}

void AScalarFieldPlayerController::BeginPlay() {
	Super::BeginPlay();

	check(_idleStateClass != nullptr);
	_state = NewObject<UIdleState>(this, _idleStateClass, "IdleState");
}

void AScalarFieldPlayerController::_onSetDestinationPressed() {
	const auto newState = _state->OnLMBPress(this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onSetDestinationReleased() {
	const auto newState = _state->OnLMBRelease(this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onSkill1Cast() {
	if (const auto sfCharacter = Cast<AScalarFieldCharacter>(GetPawn())) {
		sfCharacter->ExecuteSkillAtKey(1);
	}
}

void AScalarFieldPlayerController::_onSkill2Cast() {
	if (const auto sfCharacter = Cast<AScalarFieldCharacter>(GetPawn())) {
		sfCharacter->ExecuteSkillAtKey(2);
	}
}

void AScalarFieldPlayerController::_onSkill3Cast() {
	if (const auto sfCharacter = Cast<AScalarFieldCharacter>(GetPawn())) {
		sfCharacter->ExecuteSkillAtKey(3);
	}
}

void AScalarFieldPlayerController::_changingStateRoutine(TObjectPtr<USkillUserState> newState) {
	if (IsValid(newState)) {
		_state->OnLeave(this);
		_state = newState;
		_state->OnEnter(this);
	}
}
