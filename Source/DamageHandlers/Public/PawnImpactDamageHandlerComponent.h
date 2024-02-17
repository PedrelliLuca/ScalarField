// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "ImpactDamageHandlerInterface.h"

#include "PawnImpactDamageHandlerComponent.generated.h"

class UHealthComponent;
class UNavigationSystemV1;
class UPathFollowingComponent;
class UPawnMovementComponent;

UCLASS(ClassGroup = (DamageHandlers), meta = (BlueprintSpawnableComponent))
class DAMAGEHANDLERS_API UPawnImpactDamageHandlerComponent : public UActorComponent, public IImpactDamageHandler {
    GENERATED_BODY()

public:
    UPawnImpactDamageHandlerComponent();

protected:
    void BeginPlay() override;

public:
    void HandleImpact(const FVector& velocity, float damage) override;

    TWeakObjectPtr<UHealthComponent> _healthC;

    TWeakObjectPtr<UPathFollowingComponent> _pathFollowingC;
    TWeakObjectPtr<UNavigationSystemV1> _navSys;
    TWeakObjectPtr<UPawnMovementComponent> _pawnMovementC;
};
