// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MovementCommandSetter.h"
#include "PlayerMovementCommand.h"

#include "PlayerMovementCommandComponent.generated.h"

UCLASS()
class MOVEMENTCOMMAND_API UPlayerMovementCommandComponent : public UActorComponent, public IMovementCommandSetter {
    GENERATED_BODY()

public:
    bool IsInMovementMode(EMovementCommandMode mode) const override { return _activeMovementMode == mode; }
    void SetMovementMode(EMovementCommandMode mode) override;
    void SetDefaultMovementMode() override { SetMovementMode(_defaultMovementMode); }

    void SetDestination(const FVector& destination) override;
    void StopMovement() override;
    void MovementTick(float deltaTime) override;

protected:
    void BeginPlay() override;
    
private:
    TObjectPtr<UPlayerMovementCommand> _getMovementCommand();
    
    UPROPERTY(EditDefaultsOnly, Category = "Movement modalities")
    TMap<EMovementCommandMode, TSubclassOf<UPlayerMovementCommand>> _modesToCommandClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Movement modalities")
    EMovementCommandMode _defaultMovementMode;

    UPROPERTY()
    TObjectPtr<UPlayerMovementCommand> _activeMovementCommand;

    EMovementCommandMode _activeMovementMode;

    TWeakObjectPtr<APlayerController> _ownerPC = nullptr;    
};
