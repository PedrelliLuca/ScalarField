// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIMovementCommand.generated.h"

UCLASS(NotBlueprintable, Abstract)
class UAIMovementCommand : public UObject {
	GENERATED_BODY()

public:
	virtual void OnSetDestination(const TObjectPtr<AAIController>& aiController) PURE_VIRTUAL(UAIMovementCommand::OnSetDestination, return;);
	virtual void OnStopMovement(const TObjectPtr<AAIController>& aiController) PURE_VIRTUAL(UAIMovementCommand::OnStopMovement, return;);
	virtual void OnMovementTick(const TObjectPtr<AAIController>& aiController) PURE_VIRTUAL(UAIMovementCommand::OnMovementTick, return;);
};
