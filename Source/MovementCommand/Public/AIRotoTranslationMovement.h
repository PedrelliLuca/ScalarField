// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIMovementCommand.h"

#include "AIRotoTranslationMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UAIRotoTranslationMovement : public UAIMovementCommand {
	GENERATED_BODY()

public:
	void OnSetDestination(const TObjectPtr<AAIController>& aiController) override;
	void OnStopMovement(const TObjectPtr<AAIController>& aiController) override;
	void OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) override;
};