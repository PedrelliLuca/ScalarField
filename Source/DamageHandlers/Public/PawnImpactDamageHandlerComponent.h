// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "ImpactDamageHandlerInterface.h"

#include "PawnImpactDamageHandlerComponent.generated.h"

UCLASS(ClassGroup = (DamageHandlers), meta = (BlueprintSpawnableComponent))
class DAMAGEHANDLERS_API UPawnImpactDamageHandlerComponent : public UActorComponent, public IImpactDamageHandler {
    GENERATED_BODY()

public:
    void HandleImpact(const FVector& velocity, double damage, AActor* damageCauser) override;
};
