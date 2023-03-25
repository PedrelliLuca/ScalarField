// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIMovementCommand.h"

#include "AIRotationOnlyMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UAIRotationOnlyMovement : public UAIMovementCommand {
	GENERATED_BODY()

public:
	void OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) final;
	void OnStopMovement(const TObjectPtr<AAIController>& aiController) final;
	void OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) final;

private:
	/* Angular velocity of the pawn in degrees per second. */
	UPROPERTY(EditDefaultsOnly, Category = "Rotation Only")
	float _degreesPerSecond = 60.0f;

	double _absDegreesToRotate = 0.0;
	double _rotationSign = 1.0;
	
	double _degreesSoFar = 0.0;
};