// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIMovementCommand.h"
#include "Components/ActorComponent.h"
#include "MovementCommandSetter.h"
#include "Parameters/MovementParameters.h"

#include "AIMovementCommandComponent.generated.h"

UCLASS()
class MOVEMENTCOMMAND_API UAIMovementCommandComponent : public UActorComponent, public IMovementCommandSetter {
    GENERATED_BODY()

public:
    bool IsInMovementMode(EMovementCommandMode mode) const override { return _activeMovementMode == mode; }
    void SetMovementMode(EMovementCommandMode mode) override;
    void SetDefaultMovementMode() override { SetMovementMode(_defaultMovementMode); }

    void SetDestination(const FVector& destination) override;
    void StopMovement() override;
    void MovementTick(float deltaTime) override;

    void SetMovementParameters(const FMovementParameters& params);

    bool IsMoving() const {
        const auto activeCmd = _modesToCommands.Find(_activeMovementMode);
        check(activeCmd != nullptr);
        return (*activeCmd)->IsMoving();
    }

protected:
    void BeginPlay() override;

private:
    TObjectPtr<UAIMovementCommand> _getMovementCommand();

    void _onActiveMovementCmdStatusChange(bool newIsMoving);

    UPROPERTY(EditDefaultsOnly, Category = "Movement modalities")
    TMap<EMovementCommandMode, TSubclassOf<UAIMovementCommand>> _modesToCommandClasses;

    /** \brief Cache for already-created commands. Please UPROPERTY() TMap, don't fail me. */
    UPROPERTY()
    TMap<EMovementCommandMode, TObjectPtr<UAIMovementCommand>> _modesToCommands;

    UPROPERTY(EditDefaultsOnly, Category = "Movement modalities")
    EMovementCommandMode _defaultMovementMode;

    EMovementCommandMode _activeMovementMode = EMovementCommandMode::MCM_None;

    TWeakObjectPtr<AAIController> _ownerAIC = nullptr;
};
