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

class UNiagaraSystem;

/**
 * \brief Stores all the data related to what the player is currently interacting with.
 */
USTRUCT()
struct FPlayerInteractionData {
	GENERATED_BODY()

	TWeakObjectPtr<UInteractionComponent> InteractableBeingFocused = nullptr;
	TWeakObjectPtr<UInteractionComponent> InteractableBeingInteracted = nullptr;
	double TimestampOfLastFocusCheck = 0.0;
};

UCLASS()
class AScalarFieldPlayerController : public APlayerController, public IInteractorInterface {
	GENERATED_BODY()

public:
	AScalarFieldPlayerController();

	TWeakObjectPtr<UHUDWidget> GetGameplayHUD() { return _hudWidget; }

	bool PerformInteractionCheck() override;

	void PerformFocusCheck() override;
	bool IsInteracting() const override;
	double GetTimeLeftBeforeInteraction() const override;

protected:
	void PlayerTick(float deltaTime) override;
	void SetupInputComponent() override;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void EndInteraction() override;

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
	void _endFocus() override;
	const TWeakObjectPtr<UInteractionComponent>& _getInteractableBeingFocused() const { return _interactionData.InteractableBeingFocused; }

	// Interaction functions
	void _onInteractionInput();
	void _interact() override;
	const TWeakObjectPtr<UInteractionComponent>& _getInteractableBeingInteracted() const { return _interactionData.InteractableBeingInteracted; }
	
	UPROPERTY()
	TObjectPtr<USkillUserState> _state;

	UPROPERTY(VisibleAnywhere, Category = "Movement Commands")
	TObjectPtr<UPlayerMovementCommandComponent> _movementCommandC;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UHUDWidget> _hudWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UHUDWidget> _hudWidget = nullptr;

	FPlayerInteractionData _interactionData{};
	static constexpr double INTERACTION_TRACE_LENGTH = 100000.0;
	FTimerHandle _interactionTimerHandle{};

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	double _timeBetweenFocusChecks = 0.1;

	bool _bIsTacticalPauseOn = false;
};


