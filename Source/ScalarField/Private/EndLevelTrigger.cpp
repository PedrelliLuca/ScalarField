// Fill out your copyright notice in the Description page of Project Settings.

#include "EndLevelTrigger.h"

#include "Kismet/GameplayStatics.h"

void AEndLevelTrigger::BeginPlay() {
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AEndLevelTrigger::_onPlayerOverlap);
}

void AEndLevelTrigger::_onPlayerOverlap(AActor* overlappedActor, AActor* otherActor) {
    if (const auto pawn = Cast<APawn>(otherActor); IsValid(otherActor)) {
        if (const auto playerController = Cast<APlayerController>(pawn->GetController()); IsValid(playerController)) {
            UGameplayStatics::OpenLevel(GetWorld(), FName{"SnowyMountain"});
        }
    }
}
