// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapGridOperations.h"

#include "DrawDebugHelpers.h"
#include "HeatmapGrid.h"

namespace HeatmapGrid {
FHeatmapGrid _grid;

void Initialize(FHeatmapParameters&& params) {
    FHeatmapGrid grid;
    FHeatmapGridAttributes attributes;
    attributes.CellsExtent = params.ExtentCells;
    attributes.NumbersOfCells = params.NumberCells;

    const FVector2D& gridLocation = params.LocationGrid;
    const float heatmapExtensionX = params.NumberCells.X * params.ExtentCells.X;
    const float heatmapExtensionY = params.NumberCells.Y * params.ExtentCells.Y;
    attributes.BottomLeftCorner = FVector2D(gridLocation.X - heatmapExtensionX, gridLocation.Y - heatmapExtensionY);
    const auto firstCellLocation = FVector2D(attributes.BottomLeftCorner.X + params.ExtentCells.X, attributes.BottomLeftCorner.Y + params.ExtentCells.Y);

    grid.Attributes = MakeUnique<FHeatmapGridAttributes>(MoveTemp(attributes));

    const int32 totalNumberOfCells = params.NumberCells.X * params.NumberCells.Y;
    grid.Locations.Reserve(totalNumberOfCells);
    grid.Temperatures.Reserve(totalNumberOfCells);

    for (int32 j = 0; j < params.NumberCells.Y; ++j) {
        const float locationOffsetY = j * params.ExtentCells.Y * 2.0f;
        for (int32 i = 0; i < params.NumberCells.X; ++i) {
            const float locationOffsetX = i * params.ExtentCells.X * 2.0f;
            const auto locationOffset = FVector2D(locationOffsetX, locationOffsetY);
            grid.Locations.Emplace(firstCellLocation + locationOffset);
            grid.Temperatures.Emplace(params.InitialTemperature);
        }
    }

    _grid = MoveTemp(grid);
}

bool _rectangleCircleIntersection(const FVector2D rectangleLocation, const FVector2D rectangleExtension, const FVector2D circleLocation, const float radius) {
    const auto distance = FVector2D(FMath::Abs(circleLocation.X - rectangleLocation.X), FMath::Abs(circleLocation.Y - rectangleLocation.Y));

    if (distance.X > (rectangleExtension.X + radius) || distance.Y > (rectangleExtension.Y + radius)) {
        return false;
    }

    if (distance.X <= rectangleExtension.X || distance.Y <= rectangleExtension.Y) {
        return true;
    }

    // If you're here it means that:
    // rectangleExtension.X < distance.X < rectangleExtension.X + radius
    // rectangleExtension.Y < distance.Y < rectangleExtension.Y + radius
    const double cornerDistanceSquared = FMath::Pow(distance.X - rectangleExtension.X, 2.0) + FMath::Pow(distance.Y - rectangleExtension.Y, 2.0);
    return cornerDistanceSquared <= radius * radius;
}

void Interact(const FVector2D interactorWorldLocation, const float interactionRange) {
    const FVector2D& cellsExtent = _grid.Attributes->CellsExtent;
    const auto cellsSize = cellsExtent * 2.0;
    const FIntVector2& numbersOfCells = _grid.Attributes->NumbersOfCells;

    // From World Space to Grid Space
    const FVector2D interactorGridLocation = interactorWorldLocation - _grid.Attributes->BottomLeftCorner;

    // The cell (i, j) containing the bottom-left corner of the square enclosing the interaction circle
    const FVector2D bottomLeftLocation = interactorGridLocation - interactionRange * FVector2D::UnitVector;
    auto bottomLeftCoordinates = FIntVector2(bottomLeftLocation.X / cellsSize.X, bottomLeftLocation.Y / cellsSize.Y);
    bottomLeftCoordinates = FIntVector2(FMath::Max(bottomLeftCoordinates.X, 0), FMath::Max(bottomLeftCoordinates.Y, 0));

    // The cell (i, j) containing the top-right corner of the square enclosing the interaction circle
    const FVector2D topRightLocation = interactorGridLocation + interactionRange * FVector2D::UnitVector;
    auto topRightCoordinates = FIntVector2(topRightLocation.X / cellsSize.X, topRightLocation.Y / cellsSize.Y);
    topRightCoordinates = FIntVector2(FMath::Min(topRightCoordinates.X, numbersOfCells.X - 1), FMath::Min(topRightCoordinates.Y, numbersOfCells.Y - 1));

    check(bottomLeftCoordinates.X <= topRightCoordinates.X && bottomLeftCoordinates.Y <= topRightCoordinates.Y);

    auto convert2DTo1D = [nRows = numbersOfCells.X](int32 i, int32 j) -> int32 { return j * nRows + i; };

    // Looping over cells intersecting the enclosing square (only these can intersect with the interaction circle)
    for (int32 j = bottomLeftCoordinates.Y; j <= topRightCoordinates.Y; ++j) {
        for (int32 i = bottomLeftCoordinates.X; i <= topRightCoordinates.X; ++i) {
            const int32 k = convert2DTo1D(i, j);
            if (_rectangleCircleIntersection(_grid.Locations[k], cellsExtent, interactorWorldLocation, interactionRange))
            {
                // TODO: heat exchange
                _grid.Temperatures[k] = 1000.0f;
            }
        }
    }
}

const TArray<float>& GetTemperatures() {
    return _grid.Temperatures;
}

void DrawDebugHeatmap(const UWorld* const world, const float height) {
    const auto extent3D = FVector(_grid.Attributes->CellsExtent, 100.0f);

    const int32 nCells = _grid.Locations.Num();
    const float colorStep = 255.0f / nCells;
    auto color = FColor::Red;

    for (const FVector2D& location : _grid.Locations) {
        const auto location3D = FVector(location, height);
        DrawDebugBox(world, location3D, extent3D, color, false, 2.0f);

        // TODO: this is broken, as increments < 1 cannot be appreciated. You should use a color map
        // similar to the one in HeatmapPresenterComponent.cpp. This function should be in a new debug
        // component in ThermodynamicsPresenter or, at the very least, within HeatmapPresenterComponent. 
        color.R -= colorStep;
        color.B += colorStep;
    }
}

} // namespace HeatmapGrid
