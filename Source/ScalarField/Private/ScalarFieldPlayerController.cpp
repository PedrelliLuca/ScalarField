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

	_checkIfSomeInteractableIsFocused();
	
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

	InputComponent->BindAction("Skill1Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill1Cast);
	InputComponent->BindAction("Skill2Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill2Cast);
	InputComponent->BindAction("Skill3Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill3Cast);
	InputComponent->BindAction("Skill4Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill4Cast);
	InputComponent->BindAction("Skill5Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill5Cast);
	InputComponent->BindAction("AbortCast", IE_Pressed, this, &AScalarFieldPlayerController::_onCastAborted);
	InputComponent->BindAction("SetTarget", IE_Released, this, &AScalarFieldPlayerController::_onSetTargetPressed);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AScalarFieldPlayerController::_beginInteraction);
	InputComponent->BindAction("Interact", IE_Released, this, &AScalarFieldPlayerController::_endInteraction);

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

void AScalarFieldPlayerController::_checkIfSomeInteractableIsFocused() {
	_interactionData.TimestampOfLastInteraction = GetWorld()->GetRealTimeSeconds();

	// Building the cursor trace line
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

				// Is the component the one we're already focusing? If it isn't we update the interaction data,
				// otherwise we don't do anything
				if (hitInteractionC != _interactionData.InteractableBeingFocused) {
					_setInteractableBeingFocused(MoveTemp(hitInteractionC));
				}
				return;
			}
		}
	}

	_couldntFindInteractableToFocus();
}

void AScalarFieldPlayerController::_setInteractableBeingFocused(TWeakObjectPtr<UInteractionComponent>&& newInteractionComponent) { 
	// Were we focusing something? If so, we call EndFocus on it, as we're no longer focusing it.
	if (const auto& oldInteractable = _interactionData.InteractableBeingFocused; oldInteractable.IsValid()) {
		oldInteractable->EndFocus(this);

		// No focus means that no interaction is possible. Now, if key isn't held we don't have to worry, it means that
		// _onEndInteraction() has already been called by the release of the key
		if (_interactionData.bIsInteractKeyHeld) {
			_endInteraction();
		}
	}

	// Here the actual replacement occurs
	check(newInteractionComponent.IsValid());
	_interactionData.InteractableBeingFocused = MoveTemp(newInteractionComponent);
	newInteractionComponent->BeginFocus(this);
}

void AScalarFieldPlayerController::_couldntFindInteractableToFocus() {
	// Were we focusing something? If so, we call EndFocus on it, as we're no longer focusing it.
	if (const auto oldInteractable = _interactionData.InteractableBeingFocused; oldInteractable.IsValid()) {
		oldInteractable->EndFocus(this);
		
		// No focus means that no interaction is possible. Now, if key isn't held we don't have to worry, it means that
		// _onEndInteraction() has already been called by the release of the key
		if (_interactionData.bIsInteractKeyHeld) {
			_endInteraction();
		}
	}

	// Now there's nothing we're focusing, our data must reflect that.
	_interactionData.InteractableBeingFocused = nullptr;
}

void AScalarFieldPlayerController::_beginInteraction() {
	_interactionData.bIsInteractKeyHeld = true;

	// Are we pressing the interaction key while hovering on an interactable actor?
	if (const auto& interactable = _interactionData.InteractableBeingFocused; interactable.IsValid()) {
		interactable->BeginInteraction(this);

		if (FMath::IsNearlyZero(interactable->GetInteractionTime())) {
			_interact();
		} else {
			GetWorldTimerManager().SetTimer(_interactionTimerHandle, this, &AScalarFieldPlayerController::_interact, interactable->GetInteractionTime()); 
		}
	}
}

void AScalarFieldPlayerController::_interact() {
	GetWorldTimerManager().ClearTimer(_interactionTimerHandle);
	if (const auto& interactable = _interactionData.InteractableBeingFocused; interactable.IsValid()) {
		interactable->Interact(this);
	}
}

void AScalarFieldPlayerController::_endInteraction() {
	_interactionData.bIsInteractKeyHeld = false;
	GetWorldTimerManager().ClearTimer(_interactionTimerHandle);
	
	if (const auto& interactable = _interactionData.InteractableBeingFocused; interactable.IsValid()) {
		interactable->EndInteraction(this);		
	}
}
