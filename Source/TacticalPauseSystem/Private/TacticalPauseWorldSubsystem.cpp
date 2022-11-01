// Fill out your copyright notice in the Description page of Project Settings.

#include "TacticalPauseWorldSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UTacticalPauseWorldSubsystem::ToggleWorldTacticalPauseStatus() {
	if (!_bIsTacticalPauseOn) {
		UE_LOG(LogTemp, Warning, TEXT("Tactical Pause: ON"));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.);
		_bIsTacticalPauseOn = true;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Tactical Pause: OFF"));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.);
		_bIsTacticalPauseOn = false;
	}

	_onTacticalPauseToggle.Broadcast(_bIsTacticalPauseOn);
}
