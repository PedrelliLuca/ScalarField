// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InteractorPlayerComponent.h"
#include "PlayerMovementCommandComponent.h"
#include "WidgetsPresenterComponent.h"

#include "ScalarFieldPlayerController.generated.h"

class UNiagaraSystem;

UCLASS()
class AScalarFieldPlayerController : public APlayerController {
    GENERATED_BODY()

public:
    AScalarFieldPlayerController();

protected:
    void SetupInputComponent() override;

    void BeginPlay() override;

private:
    /** Input handlers for SetDestination action. */
    void _onSetDestinationPressed();
    void _onSetDestinationReleased();

    void _onSetTargetPressed();
    void _onSkill1Cast();
    void _onSkill2Cast();
    void _onSkill3Cast();
    void _onSkill4Cast();
    void _onSkill5Cast();
    void _onSkill6Cast();
    void _onSkillAbort();

    void _onInteraction();
    void _onInventoryToggle();

    void _onTacticalPauseToggled();
    void _answerTacticalPauseToggle(bool bIsTacticalPauseOn, double currentWorldTimeDilation);

    void _onHeatmapVisualizationToggled();

    void _onControlledPawnDeath(TObjectPtr<AActor> const deadActor);

    constexpr int32 _getSkillIdxFromKey(int32 key);

    UPROPERTY(VisibleAnywhere, Category = "Movement Commands")
    TObjectPtr<UPlayerMovementCommandComponent> _movementCommandC;

    // TODO: this will have to be moved to the pawns in order to implement group control
    UPROPERTY(VisibleAnywhere, Category = "Interaction")
    TObjectPtr<UInteractorPlayerComponent> _interactorC;

    UPROPERTY(VisibleAnywhere, Category = "Widgets Presentation")
    TObjectPtr<UWidgetsPresenterComponent> _widgetsPresenterC;

    bool _bIsTacticalPauseOn = false;

    static constexpr uint32 KEY_ASSIGNABLE_SKILLS = 10;
    static constexpr int32 PROJ_LINE_LENGTH = 20000;
};
