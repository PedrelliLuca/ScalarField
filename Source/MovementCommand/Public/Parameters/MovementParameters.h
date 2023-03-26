// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Parameters/RotoTranslationMovementParameters.h"
#include "Parameters/RotationOnlyMovementParameters.h"

#include "MovementParameters.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct MOVEMENTCOMMAND_API FMovementParameters {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FRotoTranslationMovementParameters RotoTranslationMovementParameters;

	UPROPERTY(EditAnywhere)
	FRotationOnlyMovementParameters RotationOnlyMovementParameters;
};