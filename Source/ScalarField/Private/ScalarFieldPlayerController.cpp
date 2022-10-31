// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldPlayerController.h"

#include "IdleState.h"
#include "ScalarFieldCharacter.h"

AScalarFieldPlayerController::AScalarFieldPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	_movementCommandC = CreateDefaultSubobject<UPlayerMovementCommandComponent>(TEXT("Movement Command Component"));
}

void AScalarFieldPlayerController::PlayerTick(const float deltaTime) {
	Super::PlayerTick(deltaTime);

	const auto newState = _state->OnTick(deltaTime, this);
	_changingStateRoutine(newState);

	if (_state->DisablesMovement()) {
		return;
	}

	_movementCommandC->GetMovementCommand()->OnMovementTick(this, deltaTime);
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
	InputComponent->BindAction("Skill4Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill4Cast);

	InputComponent->BindAction("AbortCast", IE_Pressed, this, &AScalarFieldPlayerController::_onCastAborted);
}

void AScalarFieldPlayerController::BeginPlay() {
	Super::BeginPlay();
	_state = NewObject<UIdleState>(this, UIdleState::StaticClass());
	_movementCommandC->SetMovementMode(EMovementCommandMode::MCM_RotoTranslation);
}

void AScalarFieldPlayerController::_onSetDestinationPressed() {
	if (_state->DisablesMovement()) {
		return;
	}

	_movementCommandC->GetMovementCommand()->OnStopMovement(this);
}

void AScalarFieldPlayerController::_onSetDestinationReleased() {
	if (_state->DisablesMovement()) {
		return;
	}

	_movementCommandC->GetMovementCommand()->OnSetDestination(this);
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

void AScalarFieldPlayerController::_onSkill4Cast() {
	const auto newState = _state->OnBeginSkillExecution(4, this);
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
