// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace HeatmapGrid {
struct FHeatmapParameters {
    int32 NCellsX = 0;
    int32 NCellsY = 0;
    float SizeCellsX = 0.0f;
    float SizeCellsY = 0.0f;
    float InitialTemperature = 273.0f;
};

void Initialize(FHeatmapParameters&& heatmapParams);
} // namespace HeatmapGrid
