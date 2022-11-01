// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "TacticalPauseWorldSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTacticalPauseToggle, bool);

/*!
 * \brief Toggles the game's "tactical pause" by toggling the world time dilation between 0 and 1.
 */
UCLASS()
class TACTICALPAUSESYSTEM_API UTacticalPauseWorldSubsystem : public UWorldSubsystem {
	GENERATED_BODY()

public:
	void ToggleWorldTacticalPauseStatus();
	FOnTacticalPauseToggle& OnTacticalPauseToggle() { return _onTacticalPauseToggle; }
	
private:
	bool _bIsTacticalPauseOn = false;

	FOnTacticalPauseToggle _onTacticalPauseToggle;
};