// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"

void UPlayerRotoTranslationMovement::OnSetDestination(
    TObjectPtr<APlayerController> const& playerController, FVector const& destination, FPlayerInputData const& inputData) {
    // Player is no longer pressing the input
    _bInputPressed = false;

    // If it was a short press
    if (_followTime <= _shortPressThreshold) {
        // We move there and spawn some particles
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(playerController, destination);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this, _fxCursor, destination, FRotator::ZeroRotator, FVector::OneVector, true, true, ENCPoolMethod::None, true);
    }
}

void UPlayerRotoTranslationMovement::OnStopMovement(TObjectPtr<APlayerController> const& playerController) {
    // We flag that the input is being pressed
    _bInputPressed = true;
    // Just in case the character was moving because of a previous short press we stop it
    playerController->StopMovement();
}

void UPlayerRotoTranslationMovement::OnMovementTick(TObjectPtr<APlayerController> const& playerController, float deltaTime) {
    if (!_bInputPressed) {
        _followTime = 0;
        return;
    }

    _followTime += deltaTime;

    // We look for the location in the world where the player has pressed the input
    FHitResult hit;
    playerController->GetHitResultUnderCursor(ECC_Visibility, true, hit);
    FVector const hitLocation = hit.Location;

    // Direct the Pawn towards that location
    if (auto const pawn = playerController->GetPawn()) {
        FVector const worldDirection = (hitLocation - pawn->GetActorLocation()).GetSafeNormal();
        pawn->AddMovementInput(worldDirection, 1.f, false);
    }
}
