// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MovementCommandMode.h"

#include "MovementCommandSetter.generated.h"

UINTERFACE(MinimalAPI)
class UMovementCommandSetter : public UInterface {
	GENERATED_BODY()
};


/**
 * 
 */
class MOVEMENTCOMMAND_API IMovementCommandSetter {
	GENERATED_BODY()
	
public:
	virtual bool IsInMovementMode(EMovementCommandMode mode) const = 0;
	virtual void SetMovementMode(EMovementCommandMode mode) = 0;
	virtual void SetDefaultMovementMode() = 0;
};
