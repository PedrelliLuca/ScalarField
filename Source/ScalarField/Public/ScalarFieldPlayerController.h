// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IdleState.h"
#include "NiagaraSystem.h"
#include "SkillUserState.h"
#include "Templates/SubclassOf.h"

#include "ScalarFieldPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AScalarFieldPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AScalarFieldPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 _bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	void PlayerTick(float deltaTime) override;
	void SetupInputComponent() override;
	// End PlayerController interface

	void BeginPlay() override;

protected:
	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UNiagaraSystem* _fxCursor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	double _shortPressThreshold = 1.;

private:
	/** Input handlers for SetDestination action. */
	void _onSetDestinationPressed();
	void _onSetDestinationReleased();

	void _onSetTargetPressed();
	void _onSkill1Cast();
	void _onSkill2Cast();
	void _onSkill3Cast();

	void _changingStateRoutine(TObjectPtr<USkillUserState> newState);

	bool _bInputPressed; // Input is being pressed
	double _followTime; // For how long it has been pressed

	UPROPERTY()
	TObjectPtr<USkillUserState> _state;
};


