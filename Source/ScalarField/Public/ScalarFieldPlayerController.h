// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerMovementCommandComponent.h"
#include "SkillUserState.h"

#include "ScalarFieldPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AScalarFieldPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AScalarFieldPlayerController();

protected:
	// Begin PlayerController interface
	void PlayerTick(float deltaTime) override;
	void SetupInputComponent() override;
	// End PlayerController interface

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

	void _changingStateRoutine(TObjectPtr<USkillUserState> newState);

	UPROPERTY()
	TObjectPtr<USkillUserState> _state;

	UPROPERTY(VisibleAnywhere, Category = "Movement Commands")
	TObjectPtr<UPlayerMovementCommandComponent> _movementCommandC;
};


