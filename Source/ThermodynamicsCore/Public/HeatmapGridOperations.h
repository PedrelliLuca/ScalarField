// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace HeatmapGrid {
struct FHeatmapParameters {
    FVector2D LocationGrid = FVector2D::ZeroVector;
    FVector2D ExtentCells = FVector2D::ZeroVector;
    FIntVector2 NumberCells = FIntVector2(0, 0);
    float InitialTemperature = 273.0f;
};

void Initialize(FHeatmapParameters&& heatmapParams);

void Interact(FVector2D interactorWorldLocation, float interactionRange);

THERMODYNAMICSCORE_API const TArray<float>& GetTemperatures();

void DrawDebugHeatmap(const UWorld* world, float height);
} // namespace HeatmapGrid
