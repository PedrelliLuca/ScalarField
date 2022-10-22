// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IdleState.h"
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

	/** Input handlers for SetDestination action. */
	void _onSetDestinationPressed();
	void _onSetDestinationReleased();

private:
	void _onSkill1Cast();
	void _onSkill2Cast();
	void _onSkill3Cast();

	void _changingStateRoutine(TObjectPtr<USkillUserState> newState);

	

	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<UIdleState> _idleStateClass = nullptr;

	UPROPERTY()
	TObjectPtr<USkillUserState> _state;
};


