// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ImpactDamageHandlerInterface.h"

#include "PawnImpactDamageHandlerComponent.generated.h"


UCLASS()
class DAMAGEHANDLERS_API UPawnImpactDamageHandlerComponent : public UActorComponent, public IImpactDamageHandler {
	GENERATED_BODY()

public:	
	void HandleImpact(const FVector& velocity, double damage, AActor* damageCauser) override;
};
