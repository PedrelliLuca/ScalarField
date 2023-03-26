// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RotoTranslationMovementParameters.generated.h"

USTRUCT(Blueprintable)
struct MOVEMENTCOMMAND_API FRotoTranslationMovementParameters {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f))
    float AcceptanceRadius = 150.0f;

    UPROPERTY(EditAnywhere)
    bool AllowPartialPath = true;
};
