// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIMovementCommand.h"

#include "AIRotationOnlyMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UAIRotationOnlyMovement : public UAIMovementCommand {
	GENERATED_BODY()

public:
	void OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) override;
	void OnStopMovement(const TObjectPtr<AAIController>& aiController) override;
	void OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) override;
};