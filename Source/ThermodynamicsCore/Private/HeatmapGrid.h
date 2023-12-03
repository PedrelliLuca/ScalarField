// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace HeatmapGrid {
struct FHeatmapGrid {
    TArray<float> Temperatures;
    TArray<FVector> Positions;
    FIntVector2 Sizes;
};
}
