// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImpactDamageHandlerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UImpactDamageHandler : public UInterface {
	GENERATED_BODY()
};

/* Interface representing an inventory. */
class IImpactDamageHandler {
	GENERATED_BODY()

public:
	virtual void HandleImpact(const FVector& velocity, double damage, AActor* damageCauser) = 0;
};
