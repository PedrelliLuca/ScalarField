// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PauseCommandInterface.h"
#include "Subsystems/WorldSubsystem.h"

#include "TacticalPauseWorldSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTacticalPauseToggle, bool, double);

/*!
 * \brief Toggles the game's "tactical pause" by toggling the world time dilation between 0 and 1.
 */
UCLASS()
class TACTICALPAUSESYSTEM_API UTacticalPauseWorldSubsystem : public UWorldSubsystem {
	GENERATED_BODY()

public:
	void OnWorldBeginPlay(UWorld& InWorld) override;

	bool IsTacticalPauseOn() const { return _bIsTacticalPauseOn; }
	void ToggleWorldTacticalPauseStatus();
	
	FOnTacticalPauseToggle& OnTacticalPauseToggle() { return _onTacticalPauseToggle; }

	void SetPauseOffCommand(TScriptInterface<IPauseCommand>&& pauseOffCommand);
	
private:
	UPROPERTY()
	TScriptInterface<IPauseCommand> _pauseOffCommand = nullptr;
	
	bool _bIsTacticalPauseOn = false;

	float _tacticalPauseTimeDilation = 0.;

	FOnTacticalPauseToggle _onTacticalPauseToggle;
};