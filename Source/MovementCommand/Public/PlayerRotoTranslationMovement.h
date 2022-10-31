// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerMovementCommand.h"

#include "PlayerRotoTranslationMovement.generated.h"

UCLASS()
class MOVEMENTCOMMAND_API UPlayerRotoTranslationMovement : public UPlayerMovementCommand {
	GENERATED_BODY()

public:
	void OnSetDestination(const TObjectPtr<APlayerController>& playerController) override;
	void OnStopMovement(const TObjectPtr<APlayerController>& playerController) override;
	void OnMovementTick(const TObjectPtr<APlayerController>& playerController) override;
};