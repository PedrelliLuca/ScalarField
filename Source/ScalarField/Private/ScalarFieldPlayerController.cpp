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
}

void AScalarFieldPlayerController::PlayerTick(const float deltaTime) {
	Super::PlayerTick(deltaTime);

	_performInteractionCheck();
	
	// We handled the input with the Super:: call. If the tacticalPause is on, we skip the FSM's and movement cmd tick
	if (_bIsTacticalPauseOn) {
		return;
	}
	
	const auto newState = _state->OnTick(deltaTime, this);
	_changingStateRoutine(newState);

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
	InputComponent->BindAction("Skill5Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill5Cast);

	InputComponent->BindAction("AbortCast", IE_Pressed, this, &AScalarFieldPlayerController::_onCastAborted);

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
	const auto newState = _state->OnSkillExecutionAborted(this);
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

void AScalarFieldPlayerController::_performInteractionCheck() {
	_interactionData.TimestampOfLastInteraction = GetWorld()->GetRealTimeSeconds();

	// Building the trace line
	FVector cursorLoc{};
	FVector cursorDir{};
	DeprojectMousePositionToWorld(cursorLoc, cursorDir);
	const auto& traceStart = cursorLoc;
	const auto traceEnd = traceStart + cursorDir * INTERACTION_TRACE_LENGTH;

	// Did we find an actor blocking the visibility channel?
	FHitResult traceHit{};
	if (GetWorld()->LineTraceSingleByChannel(traceHit, traceStart, traceEnd, ECollisionChannel::ECC_Visibility) && traceHit.GetActor()) {
		// Does the actor we found have an interaction component?
		if (TWeakObjectPtr<UInteractionComponent> hitInteractionC = traceHit.GetActor()->FindComponentByClass<UInteractionComponent>(); hitInteractionC.IsValid()) {
			const auto pawn = GetPawn();
			check(IsValid(pawn));

			// Is the component within reach?
			const double distance = (traceHit.ImpactPoint - pawn->GetActorLocation()).Size();
			if (distance <= hitInteractionC->GetInteractionDistance()) {

				// Is the component the one we're already interacting with? If it isn't we update the interaction data,
				// otherwise we don't do anything
				if (hitInteractionC != _interactionData.ComponentBeingInteracted) {
					_replaceComponentBeingInteracted(MoveTemp(hitInteractionC));
				}
				return;
			}
		}
	}

	_forgetInteractionComponent();
}

void AScalarFieldPlayerController::_replaceComponentBeingInteracted(TWeakObjectPtr<UInteractionComponent>&& newInteractionComponent) {
	check(newInteractionComponent.IsValid());

	UE_LOG(LogTemp, Error, TEXT("Interaction has started"));
	newInteractionComponent->SetHiddenInGame(false);
	_interactionData.ComponentBeingInteracted = MoveTemp(newInteractionComponent);	
}

void AScalarFieldPlayerController::_forgetInteractionComponent() {
	if (_interactionData.ComponentBeingInteracted.IsValid()) {
		UE_LOG(LogTemp, Error, TEXT("Interaction is over"));
		_interactionData.ComponentBeingInteracted->SetHiddenInGame(true);
		_interactionData.ComponentBeingInteracted = nullptr;
	}
}
