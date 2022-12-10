// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InteractorPlayerComponent.h"
#include "PlayerMovementCommandComponent.h"
#include "SkillUserState.h"
#include "WidgetsPresenterComponent.h"

#include "ScalarFieldPlayerController.generated.h"

class UNiagaraSystem;

UCLASS()
class AScalarFieldPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AScalarFieldPlayerController();

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
	
	void _onInteractionInput();

	void _onToggleInventoryInput();

	void _onTacticalPauseToggled();
	void _answerTacticalPauseToggle(bool bIsTacticalPauseOn, double currentWorldTimeDilation);

	void _changingStateRoutine(TObjectPtr<USkillUserState> newState);
	
	UPROPERTY()
	TObjectPtr<USkillUserState> _state;

	UPROPERTY(VisibleAnywhere, Category = "Movement Commands")
	TObjectPtr<UPlayerMovementCommandComponent> _movementCommandC;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<UInteractorPlayerComponent> _interactorC;

	UPROPERTY(VisibleAnywhere, Category = "Widgets Presentation")
	TObjectPtr<UWidgetsPresenterComponent> _widgetsPresenter;

	bool _bIsTacticalPauseOn = false;
};


