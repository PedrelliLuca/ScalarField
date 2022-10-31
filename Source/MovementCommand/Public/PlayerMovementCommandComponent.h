// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementCommandSetter.h"
#include "PlayerMovementCommand.h"

#include "PlayerMovementCommandComponent.generated.h"

UCLASS()
class MOVEMENTCOMMAND_API UPlayerMovementCommandComponent : public UActorComponent, public IMovementCommandSetter {
	GENERATED_BODY()

public:	
	bool IsInMovementMode(EMovementCommandMode mode) const override { return _activeMovementMode == mode; }
	void SetMovementMode(EMovementCommandMode mode) override;

	TObjectPtr<UPlayerMovementCommand> GetMovementCommand() {
		// Did you set the movement mode before calling this?
		check(IsValid(_activeMovementCommand));
		return _activeMovementCommand;
	}

private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement modalities")
	TMap<EMovementCommandMode, TSubclassOf<UPlayerMovementCommand>> _modesToCommandClasses;
	
	EMovementCommandMode _activeMovementMode;

	UPROPERTY()
	TObjectPtr<UPlayerMovementCommand> _activeMovementCommand;
};