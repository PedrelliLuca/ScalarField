// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIMovementCommand.h"
#include "Components/ActorComponent.h"
#include "MovementCommandSetter.h"

#include "AIMovementCommandComponent.generated.h"

UCLASS()
class MOVEMENTCOMMAND_API UAIMovementCommandComponent : public UActorComponent, public IMovementCommandSetter {
	GENERATED_BODY()

public:	
	bool IsInMovementMode(EMovementCommandMode mode) const override { return _activeMovementMode == mode; }
	void SetMovementMode(EMovementCommandMode mode) override;
	void SetDefaultMovementMode() override { SetMovementMode(_defaultMovementMode); }

	TObjectPtr<UAIMovementCommand> GetMovementCommand() {
		const auto activeCmd = _modesToCommands.Find(_activeMovementMode);
		// Did you set the movement mode before calling this?
		check(activeCmd != nullptr);
		return *activeCmd;
	}

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement modalities")
	TMap<EMovementCommandMode, TSubclassOf<UAIMovementCommand>> _modesToCommandClasses;

	/** \brief Cache for already-created commands. Please UPROPERTY() TMap, don't fail me. */
	UPROPERTY()
	TMap<EMovementCommandMode, TObjectPtr<UAIMovementCommand>> _modesToCommands;

	UPROPERTY(EditDefaultsOnly, Category = "Movement modalities")
	EMovementCommandMode _defaultMovementMode;
	
	EMovementCommandMode _activeMovementMode = EMovementCommandMode::MCM_None;

	TWeakObjectPtr<AAIController> _ownerAIController = nullptr;
};