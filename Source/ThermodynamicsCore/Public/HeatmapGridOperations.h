// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UCollisionsCollectionComponent;

namespace HeatmapGrid {
struct FHeatmapParameters {
    FVector2D LocationGrid = FVector2D::ZeroVector;
    FVector2D ExtentCells = FVector2D::ZeroVector;
    FIntVector2 NumberCells = FIntVector2(0, 0);
    float InitialTemperature = 273.0f;
    float HeatCapacity = 1.0f;
};

void Initialize(FHeatmapParameters&& heatmapParams);
void Deinitialize();

// Realizes a thermodynamic interaction between the given interactor (transform, thermodynamicsCollision, temperature) and the grid in the time deltaTime.
// Returns the grid-normalized currDeltaT, i.e. the sum of all the currDeltaTs with the interacting cells divided by the number of interacting cells.
float Interact(FTransform const& interactorTransform, UCollisionsCollectionComponent* collisionsCollection, float interactorTemperature, float deltaTime);

// Realizes thermodynamic interactions between each cell and its orthogonal neighbors.
void SelfInteract(float deltaTime);

THERMODYNAMICSCORE_API const TArray<float>& GetTemperatures();

void DrawDebugHeatmap(UWorld const* world, float height);
} // namespace HeatmapGrid
