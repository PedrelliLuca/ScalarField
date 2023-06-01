// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "PlayerMovementCommand.h"

#include "PlayerRotoTranslationMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UPlayerRotoTranslationMovement : public UPlayerMovementCommand {
    GENERATED_BODY()

public:
    void OnSetDestination(const TObjectPtr<APlayerController>& playerController, const FVector& destination, const FPlayerInputData& inputData) override;
    void OnStopMovement(const TObjectPtr<APlayerController>& playerController) override;
    void OnMovementTick(const TObjectPtr<APlayerController>& playerController, float deltaTime) override;

private:
    bool _bInputPressed; // Input is being pressed
    double _followTime;  // For how long it has been pressed

    /** True if the controlled character should navigate to the mouse cursor. */
    uint32 _bMoveToMouseCursor : 1;

    UPROPERTY(EditAnywhere, Category = "Input")
    double _shortPressThreshold = 1.;

    /** FX Class that we will spawn when clicking */
    UPROPERTY(EditAnywhere, Category = "Input")
    UNiagaraSystem* _fxCursor;
};
