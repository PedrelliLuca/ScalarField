// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPlayerRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"

void UNewPlayerRotoTranslationMovement::OnSetDestination(const TObjectPtr<APlayerController>& playerController, const FVector& destination, const FPlayerInputData& inputData) {
    _inputEvent = inputData.SetDestinationInputEvent;

    if (_inputEvent == IE_Pressed) {
        playerController->StopMovement();
    } else if (_followTime <= _shortPressThreshold) {
        // If it was a short press we move there and spawn some particles
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(playerController, destination);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this, _fxCursor, destination, FRotator::ZeroRotator, FVector::OneVector, true, true, ENCPoolMethod::None, true);
    }
}

void UNewPlayerRotoTranslationMovement::OnStopMovement(const TObjectPtr<APlayerController>& playerController) {
    playerController->StopMovement();
}

void UNewPlayerRotoTranslationMovement::OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) {
    if (_inputEvent == IE_Released) {
        _followTime = 0;
        return;
    }

    _followTime += deltaTime;

    // We look for the location in the world where the player has pressed the input
    FHitResult hit;
    playerController->GetHitResultUnderCursor(ECC_Visibility, true, hit);
    const FVector hitLocation = hit.Location;

    // Direct the Pawn towards that location
    if (const auto pawn = playerController->GetPawn()) {
        const FVector worldDirection = (hitLocation - pawn->GetActorLocation()).GetSafeNormal();
        pawn->AddMovementInput(worldDirection, 1.f, false);
    }
}
