// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float _shortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* _fxCursor;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 _bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void _onSetDestinationPressed();
	void _onSetDestinationReleased();
	void _onTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void _onTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);

private:
	void _onSkill1Cast();
	void _onSkill2Cast();
	void _onSkill3Cast();

	bool _bInputPressed; // Input is bring pressed
	bool _bIsTouch; // Is it a touch device
	float _followTime; // For how long it has been pressed
};


