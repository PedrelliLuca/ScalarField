// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldPlayerController.h"

#include "IdleState.h"
#include "TacticalPauseWorldSubsystem.h"

AScalarFieldPlayerController::AScalarFieldPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	_movementCommandC = CreateDefaultSubobject<UPlayerMovementCommandComponent>(TEXT("Movement Command Component"));
	_stateC = CreateDefaultSubobject<UStateComponent>(TEXT("State Component"));
	_interactorC = CreateDefaultSubobject<UInteractorPlayerComponent>(TEXT("Interactor Component"));
	_widgetsPresenter = CreateDefaultSubobject<UWidgetsPresenterComponent>(TEXT("Widgets Presenter"));
}

void AScalarFieldPlayerController::PlayerTick(const float deltaTime) {
	Super::PlayerTick(deltaTime);
	
	// Generally speaking, the tick of the states should stop if the tactical pause is active. However, some states
	// are special and are not affected by it.
	if (!_bIsTacticalPauseOn || !_stateC->IsCurrentStateAffectedByPause()) {
		_stateC->PerformTickBehavior(deltaTime);
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
	InputComponent->BindAction("AbortCast", IE_Pressed, _stateC.Get(), &UStateComponent::PerformAbortBehavior);
	InputComponent->BindAction("SetTarget", IE_Released, this, &AScalarFieldPlayerController::_onSetTargetPressed);

	InputComponent->BindAction("Interact", IE_Pressed, _stateC.Get(), &UStateComponent::PerformInteractionBehavior);

	InputComponent->BindAction("ToggleInventory", IE_Pressed, _stateC.Get(), &UStateComponent::PerformInventoryToggleBehavior);

	InputComponent->BindAction("ToggleTacticalPause", IE_Released, this, &AScalarFieldPlayerController::_onTacticalPauseToggled);
}

void AScalarFieldPlayerController::BeginPlay() {
	Super::BeginPlay();
	
	const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
	pauseSubsys->OnTacticalPauseToggle().AddUObject(this, &AScalarFieldPlayerController::_answerTacticalPauseToggle);
	_bIsTacticalPauseOn = pauseSubsys->IsTacticalPauseOn();

	_widgetsPresenter->SetOwnerPlayerController(this);
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
	_stateC->PerformTargetingBehavior(hit.GetActor());
}

void AScalarFieldPlayerController::_onSkill1Cast() {
	_stateC->PerformSkillExecutionBehavior(1);
}

void AScalarFieldPlayerController::_onSkill2Cast() {
	_stateC->PerformSkillExecutionBehavior(2);
}

void AScalarFieldPlayerController::_onSkill3Cast() {
	_stateC->PerformSkillExecutionBehavior(3);
}

void AScalarFieldPlayerController::_onSkill4Cast() {
	_stateC->PerformSkillExecutionBehavior(4);
}

void AScalarFieldPlayerController::_onSkill5Cast() {
	_stateC->PerformSkillExecutionBehavior(5);
}

void AScalarFieldPlayerController::_onTacticalPauseToggled() {
	GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>()->ToggleWorldTacticalPauseStatus();
}

void AScalarFieldPlayerController::_answerTacticalPauseToggle(const bool bIsTacticalPauseOn, const double currentWorldTimeDilation) {
	/* Here we're literally overriding whatever the UTacticalPauseWorldSubsystem just did. 
	 * The PlayerController must never, ever, have its time dilation close to zero, since
	 * that would cause the player to not be able to send any kind of input. */
	CustomTimeDilation = 1. / currentWorldTimeDilation;

	_bIsTacticalPauseOn = bIsTacticalPauseOn;
}
