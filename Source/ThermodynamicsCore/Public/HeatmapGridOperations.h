// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace HeatmapGrid {
struct FHeatmapParameters {
    FVector2D LocationGrid = FVector2D::ZeroVector;
    FVector2D ExtentCells = FVector2D::ZeroVector;
    FIntVector2 NumberCells = FIntVector2(0, 0);
    float InitialTemperature = 273.0f;
    float HeatCapacity = 1.0f;
};

void Initialize(FHeatmapParameters&& heatmapParams);

// Realizes a thermodynamic interaction between the given interactor (location, range, temperature) and the grid in the time deltaTime.
// Returns the grid-normalized currDeltaT, i.e. the sum of all the currDeltaTs with the interacting cells divided by the number of interacting cells.
float Interact(FVector2D interactorWorldLocation, float interactorRange, float interactorTemperature, float deltaTime);

// Realizes thermodynamic interactions between each cell and its orthogonal neighbors.
void SelfInteract(float deltaTime);

THERMODYNAMICSCORE_API const TArray<float>& GetTemperatures();

void DrawDebugHeatmap(const UWorld* world, float height);
} // namespace HeatmapGrid
