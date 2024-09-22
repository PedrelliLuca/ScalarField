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
    void OnSetDestination(TObjectPtr<APlayerController> const& playerController, FVector const& destination, FPlayerInputData const& inputData) override;
    void OnStopMovement(TObjectPtr<APlayerController> const& playerController) override;
    void OnMovementTick(TObjectPtr<APlayerController> const& playerController, float deltaTime) override;

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
