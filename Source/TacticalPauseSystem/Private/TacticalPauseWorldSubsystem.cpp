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
        currentWorldTimeDilation = _tacticalPauseTimeDilation;
        _bIsTacticalPauseOn = true;
    } else {
        currentWorldTimeDilation = 1.;
        _bIsTacticalPauseOn = false;

        if (IsValid(_pauseOffCommand.GetObject())) {
            _pauseOffCommand->Execute();
            _pauseOffCommand = nullptr;
        }
    }

    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), currentWorldTimeDilation);

    _onTacticalPauseToggle.Broadcast(_bIsTacticalPauseOn, currentWorldTimeDilation);
}

void UTacticalPauseWorldSubsystem::SetPauseOffCommand(TScriptInterface<IPauseCommand>&& pauseOffCommand) {
    check(IsValid(pauseOffCommand.GetObject()));
    _pauseOffCommand = MoveTemp(pauseOffCommand);
}
