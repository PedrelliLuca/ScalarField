// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HUDWidget.h"
#include "InteractionComponent.h"
#include "InteractorInterface.h"
#include "PlayerMovementCommandComponent.h"
#include "SkillUserState.h"

#include "ScalarFieldPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

/**
 * \brief Stores all the data related to what the player is currently interacting with.
 */
USTRUCT()
struct FInteractionData {
	GENERATED_BODY()

	TWeakObjectPtr<UInteractionComponent> InteractableBeingFocused = nullptr;
	double TimestampOfLastInteraction = 0.0;
	bool bIsInteractKeyHeld = false;
};

UCLASS()
class AScalarFieldPlayerController : public APlayerController, public IInteractorInterface {
	GENERATED_BODY()

public:
	AScalarFieldPlayerController();

	TWeakObjectPtr<UHUDWidget> GetGameplayHUD() { return _hudWidget; }

protected:
	void PlayerTick(float deltaTime) override;
	void SetupInputComponent() override;

	void BeginPlay() override;

private:
	/** Input handlers for SetDestination action. */
	void _onSetDestinationPressed();
	void _onSetDestinationReleased();

	void _onSetTargetPressed();
	void _onSkill1Cast();
	void _onSkill2Cast();
	void _onSkill3Cast();
	void _onSkill4Cast();
	void _onSkill5Cast();

	void _onCastAborted();

	void _onTacticalPauseToggled();
	void _answerTacticalPauseToggle(bool bIsTacticalPauseOn, double currentWorldTimeDilation);

	void _changingStateRoutine(TObjectPtr<USkillUserState> newState);

	void _createHUD();

	// Focus functions
	void _performFocusCheck() override;
	void _setInteractableBeingFocused(TWeakObjectPtr<UInteractionComponent>&& newInteractionComponent) override;
	void _couldntFindInteractableToFocus() override;

	// Interaction functions
	void _beginInteraction() override;
	void _interact() override;
	void _endInteraction() override;
	
	UPROPERTY()
	TObjectPtr<USkillUserState> _state;

	UPROPERTY(VisibleAnywhere, Category = "Movement Commands")
	TObjectPtr<UPlayerMovementCommandComponent> _movementCommandC;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UHUDWidget> _hudWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UHUDWidget> _hudWidget = nullptr;

	FInteractionData _interactionData{};
	static constexpr double INTERACTION_TRACE_LENGTH = 100000.0;
	FTimerHandle _interactionTimerHandle{};

	bool _bIsTacticalPauseOn = false;
};


