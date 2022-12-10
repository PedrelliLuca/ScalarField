// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldPlayerController.h"

#include "HealthComponent.h"
#include "IdleState.h"
#include "ManaComponent.h"
#include "TacticalPauseWorldSubsystem.h"

AScalarFieldPlayerController::AScalarFieldPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	_movementCommandC = CreateDefaultSubobject<UPlayerMovementCommandComponent>(TEXT("Movement Command Component"));
	_interactorC = CreateDefaultSubobject<UInteractorPlayerComponent>(TEXT("Interactor Component"));
}

void AScalarFieldPlayerController::PlayerTick(const float deltaTime) {
	Super::PlayerTick(deltaTime);
	
	// Generally speaking, the tick of the states should stop if the tactical pause is active. However, some states
	// are special and are not affected by it.
	if (!_bIsTacticalPauseOn || !_state->IsTickAffectedByPause()) {
		const auto newState = _state->OnTick(deltaTime, this);
		_changingStateRoutine(newState);
	}

	// Tick of movement commands never occurs during the tactical pause.
	if (!_bIsTacticalPauseOn) {
		_movementCommandC->GetMovementCommand()->OnMovementTick(this, deltaTime);
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
	InputComponent->BindAction("Skill4Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill4Cast);
	InputComponent->BindAction("Skill5Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill5Cast);
	InputComponent->BindAction("AbortCast", IE_Pressed, this, &AScalarFieldPlayerController::_onCastAborted);
	InputComponent->BindAction("SetTarget", IE_Released, this, &AScalarFieldPlayerController::_onSetTargetPressed);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AScalarFieldPlayerController::_onInteractionInput);

	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AScalarFieldPlayerController::_onToggleInventoryInput);

	InputComponent->BindAction("ToggleTacticalPause", IE_Released, this, &AScalarFieldPlayerController::_onTacticalPauseToggled);
}

void AScalarFieldPlayerController::BeginPlay() {
	Super::BeginPlay();
	_state = NewObject<UIdleState>(this, UIdleState::StaticClass());
	_movementCommandC->SetDefaultMovementMode();

	const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
	pauseSubsys->OnTacticalPauseToggle().AddUObject(this, &AScalarFieldPlayerController::_answerTacticalPauseToggle);
	_bIsTacticalPauseOn = pauseSubsys->IsTacticalPauseOn();

	_createHUD();
}

void AScalarFieldPlayerController::_onSetDestinationPressed() {
	_movementCommandC->GetMovementCommand()->OnStopMovement(this);
}

void AScalarFieldPlayerController::_onSetDestinationReleased() {
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

void AScalarFieldPlayerController::_onSkill5Cast() {
	const auto newState = _state->OnBeginSkillExecution(5, this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onCastAborted() {
	const auto newState = _state->OnAbort(this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onTacticalPauseToggled() {
	GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>()->ToggleWorldTacticalPauseStatus();
}

void AScalarFieldPlayerController::_answerTacticalPauseToggle(const bool bIsTacticalPauseOn, const double currentWorldTimeDilation) {
	/* Here we're literally overriding whatever the UTacticalPauseWorldSubsystem just did. 
	 * The PlayerController must never, ever, have its time dilation different from 1, since
	 * that would cause the player to not be able to send any kind of input. */
	CustomTimeDilation = 1. / currentWorldTimeDilation;

	_bIsTacticalPauseOn = bIsTacticalPauseOn;
	_hudWidget->SetPauseStatus(_bIsTacticalPauseOn);
}

void AScalarFieldPlayerController::_changingStateRoutine(TObjectPtr<USkillUserState> newState) {
	if (IsValid(newState)) {
		_state->OnLeave(this);
		_state = newState;
		_state->OnEnter(this);
	}
}

void AScalarFieldPlayerController::_createHUD() {
	// Create only once
	check(!IsValid(_hudWidget));

	if (!IsValid(_hudWidgetClass)) {
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UHUDWidget class. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	_hudWidget = CreateWidget<UHUDWidget>(this, _hudWidgetClass);
    _hudWidget->AddToViewport();

	const auto pawn = GetPawn();

	const auto healthC = pawn->FindComponentByClass<const UHealthComponent>();
	check(IsValid(healthC));

	_hudWidget->SetCurrentHealth(healthC->GetCurrentHealth());
	_hudWidget->SetMaxHealth(healthC->GetMaxHealth());
	_hudWidget->SetHealthRegen(healthC->GetHealthRegen());

	const auto manaC = pawn->FindComponentByClass<const UManaComponent>();
	check(IsValid(manaC));

	_hudWidget->SetCurrentMana(manaC->GetCurrentMana());
	_hudWidget->SetMaxMana(manaC->GetMaxMana());
	_hudWidget->SetManaRegen(manaC->GetManaRegen());

	_hudWidget->SetPauseStatus(_bIsTacticalPauseOn);
}

void AScalarFieldPlayerController::_onInteractionInput() {
	const auto newState = _state->OnInteraction(this);
	_changingStateRoutine(newState);
}

void AScalarFieldPlayerController::_onToggleInventoryInput() {
	const auto newState = _state->OnToggleInventory(this);
	_changingStateRoutine(newState);
}
