// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapGridOperations.h"

#include "DrawDebugHelpers.h"
#include "HeatmapGrid.h"

namespace HeatmapGrid {
FHeatmapGrid _grid;

void Initialize(FHeatmapParameters&& params) {
    FHeatmapGrid grid;
    grid.CellExtent = params.ExtentCells;

    const FVector2D& gridLocation = params.LocationGrid;
    const float heatmapExtensionX = params.NumberCells.X * params.ExtentCells.X;
    const float heatmapExtensionY = params.NumberCells.Y * params.ExtentCells.Y;
    const auto firstCellLocation =
        FVector2D(gridLocation.X - heatmapExtensionX + params.ExtentCells.X, gridLocation.Y - heatmapExtensionY + params.ExtentCells.Y);

    const int32 totalNumberOfCells = params.NumberCells.X * params.NumberCells.Y;
    grid.Positions.Reserve(totalNumberOfCells);
    grid.Temperatures.Reserve(totalNumberOfCells);

    for (int32 j = 0; j < params.NumberCells.Y; ++j) {
        const float locationOffsetY = j * params.ExtentCells.Y * 2.0f;
        for (int32 i = 0; i < params.NumberCells.X; ++i) {
            const float locationOffsetX = i * params.ExtentCells.X * 2.0f;
            const auto locationOffset = FVector2D(locationOffsetX, locationOffsetY);
            grid.Positions.Emplace(firstCellLocation + locationOffset);
            grid.Temperatures.Emplace(params.InitialTemperature);
        }
    }

    _grid = MoveTemp(grid);
}

void DrawDebugHeatmap(const UWorld* const world, const float height) {
    const auto extent3D = FVector(_grid.CellExtent, 10.0f);

    const int32 nCells = _grid.Positions.Num();
    const float colorStep = 255.0f / nCells;
    auto color = FColor::Red;

    for (const FVector2D& position : _grid.Positions) {
        const auto position3D = FVector(position, height);
        DrawDebugBox(world, position3D, extent3D, color, false, 2.0f);

        color.R -= colorStep;
        color.B += colorStep;
    }
}

} // namespace HeatmapGrid
