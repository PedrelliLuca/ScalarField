// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerMovementCommand.generated.h"

UCLASS(NotBlueprintable, Abstract)
class UPlayerMovementCommand : public UObject {
	GENERATED_BODY()

public:
	virtual void OnSetDestination(const TObjectPtr<APlayerController>& playerController) PURE_VIRTUAL(UPlayerMovementCommand::OnSetDestination, return;);
	virtual void OnStopMovement(const TObjectPtr<APlayerController>& playerController) PURE_VIRTUAL(UPlayerMovementCommand::OnStopMovement, return;);
	virtual void OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) PURE_VIRTUAL(UPlayerMovementCommand::OnMovementTick, return;);
};
