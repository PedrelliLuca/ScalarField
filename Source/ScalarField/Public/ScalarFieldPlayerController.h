// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HUDWidget.h"
#include "InteractionComponent.h"
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

	TWeakObjectPtr<UInteractionComponent> ComponentBeingInteracted = nullptr;
	double TimestampOfLastInteraction = 0.0;
	bool bIsInteractKeyHeld = false;
};

UCLASS()
class AScalarFieldPlayerController : public APlayerController {
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

	void _performInteractionCheck();
	void _newInteractableFound(TWeakObjectPtr<UInteractionComponent>&& newInteractionComponent);
	void _newInteractableNotFound();

	UPROPERTY()
	TObjectPtr<USkillUserState> _state;

	UPROPERTY(VisibleAnywhere, Category = "Movement Commands")
	TObjectPtr<UPlayerMovementCommandComponent> _movementCommandC;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UHUDWidget> _hudWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UHUDWidget> _hudWidget = nullptr;

	FInteractionData _interactionData{};
	static constexpr double INTERACTION_TRACE_LENGTH = 10000.0;

	bool _bIsTacticalPauseOn = false;
};


