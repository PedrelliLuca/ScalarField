// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HeatMap.generated.h"

/*!
 * \brief Actor to visualize the heat map to the player
 */
UCLASS(Blueprintable)
class AHeatMap : public AActor {
    GENERATED_BODY()

public:
    AHeatMap();
};
