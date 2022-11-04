// Fill out your copyright notice in the Description page of Project Settings.

#include "TacticalPauseWorldSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UTacticalPauseWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld) {
	Super::OnWorldBeginPlay(InWorld);
	_tacticalPauseTimeDilation = SMALL_NUMBER;
}

void UTacticalPauseWorldSubsystem::ToggleWorldTacticalPauseStatus() {
	double currentWorldTimeDilation;
	if (!_bIsTacticalPauseOn) {
		UE_LOG(LogTemp, Warning, TEXT("Tactical Pause: ON"));
		currentWorldTimeDilation = _tacticalPauseTimeDilation;
		_bIsTacticalPauseOn = true;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Tactical Pause: OFF"));
		currentWorldTimeDilation = 1.;
		_bIsTacticalPauseOn = false;
	}

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), currentWorldTimeDilation);

	_onTacticalPauseToggle.Broadcast(_bIsTacticalPauseOn, currentWorldTimeDilation);
}