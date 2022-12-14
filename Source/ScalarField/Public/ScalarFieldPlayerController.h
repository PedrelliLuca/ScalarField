// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InteractorPlayerComponent.h"
#include "PlayerMovementCommandComponent.h"
#include "StateComponent.h"
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

	void _onTacticalPauseToggled();
	void _answerTacticalPauseToggle(bool bIsTacticalPauseOn, double currentWorldTimeDilation);

	UPROPERTY(VisibleAnywhere, Category = "Movement Commands")
	TObjectPtr<UPlayerMovementCommandComponent> _movementCommandC;

	UPROPERTY(VisibleAnywhere, Category = "State")
	TObjectPtr<UStateComponent> _stateC;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<UInteractorPlayerComponent> _interactorC;

	UPROPERTY(VisibleAnywhere, Category = "Widgets Presentation")
	TObjectPtr<UWidgetsPresenterComponent> _widgetsPresenterC;

	bool _bIsTacticalPauseOn = false;
};


