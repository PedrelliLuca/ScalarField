// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPlayerRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"

void UNewPlayerRotoTranslationMovement::OnSetDestination(
    TObjectPtr<APlayerController> const& playerController, FVector const& destination, FPlayerInputData const& inputData) {
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

void UNewPlayerRotoTranslationMovement::OnStopMovement(TObjectPtr<APlayerController> const& playerController) {
    playerController->StopMovement();
}

void UNewPlayerRotoTranslationMovement::OnMovementTick(TObjectPtr<APlayerController> const& playerController, float deltaTime) {
    if (_inputEvent == IE_Released) {
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
