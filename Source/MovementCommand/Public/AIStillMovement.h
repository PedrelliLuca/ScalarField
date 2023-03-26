// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIMovementCommand.h"

#include "AIStillMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UAIStillMovement : public UAIMovementCommand {
	GENERATED_BODY()

public:
	void OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) final { 
		aiController->StopMovement(); 
		_setIsMoving(false);
	}

	void OnStopMovement(const TObjectPtr<AAIController>& aiController) final { 
		aiController->StopMovement(); 
		_setIsMoving(false);
	}

	void OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) final {}

	void SetMovementParameters(const FMovementParameters& params) final {}
};