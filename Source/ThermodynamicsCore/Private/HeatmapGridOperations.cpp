// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapGridOperations.h"

#include "DrawDebugHelpers.h"
#include "HeatmapGrid.h"
#include "ThermodynamicsSubsystem.h"

namespace HeatmapGrid {
FHeatmapGrid _grid;

void Initialize(FHeatmapParameters&& params) {
    FHeatmapGrid grid;
    FHeatmapGridAttributes attributes;
    attributes.CellsExtent = params.ExtentCells;
    attributes.NumbersOfCells = params.NumberCells;
    attributes.HeatCapacity = params.HeatCapacity;

    const FVector2D& gridLocation = params.LocationGrid;
    const float heatmapExtensionX = params.NumberCells.X * params.ExtentCells.X;
    const float heatmapExtensionY = params.NumberCells.Y * params.ExtentCells.Y;
    attributes.BottomLeftCorner = FVector2D(gridLocation.X - heatmapExtensionX, gridLocation.Y - heatmapExtensionY);
    const auto firstCellLocation = FVector2D(attributes.BottomLeftCorner.X + params.ExtentCells.X, attributes.BottomLeftCorner.Y + params.ExtentCells.Y);

    grid.Attributes = MoveTemp(attributes);

    const int32 totalNumberOfCells = params.NumberCells.X * params.NumberCells.Y;
    grid.Locations.Reserve(totalNumberOfCells);
    grid.CurrentTemperatures.Reserve(totalNumberOfCells);
    grid.NextTemperatures.Reserve(totalNumberOfCells);

    for (int32 j = 0; j < params.NumberCells.Y; ++j) {
        const float locationOffsetY = j * params.ExtentCells.Y * 2.0f;
        for (int32 i = 0; i < params.NumberCells.X; ++i) {
            const float locationOffsetX = i * params.ExtentCells.X * 2.0f;
            const auto locationOffset = FVector2D(locationOffsetX, locationOffsetY);
            grid.Locations.Emplace(firstCellLocation + locationOffset);
            grid.CurrentTemperatures.Emplace(params.InitialTemperature);
            grid.NextTemperatures.Emplace(params.InitialTemperature);
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

float Interact(const FVector2D interactorWorldLocation, const float interactorRange, const float interactorTemperature, const float deltaTime) {
    const FVector2D& cellsExtent = _grid.Attributes.CellsExtent;
    const auto cellsSize = cellsExtent * 2.0;
    const FIntVector2& numbersOfCells = _grid.Attributes.NumbersOfCells;

    // From World Space to Grid Space
    const FVector2D interactorGridLocation = interactorWorldLocation - _grid.Attributes.BottomLeftCorner;

    // The cell (i, j) containing the bottom-left corner of the square enclosing the interaction circle
    const FVector2D bottomLeftLocation = interactorGridLocation - interactorRange * FVector2D::UnitVector;
    auto bottomLeftCoordinates = FIntVector2(bottomLeftLocation.X / cellsSize.X, bottomLeftLocation.Y / cellsSize.Y);
    bottomLeftCoordinates = FIntVector2(FMath::Max(bottomLeftCoordinates.X, 0), FMath::Max(bottomLeftCoordinates.Y, 0));

    // The cell (i, j) containing the top-right corner of the square enclosing the interaction circle
    const FVector2D topRightLocation = interactorGridLocation + interactorRange * FVector2D::UnitVector;
    auto topRightCoordinates = FIntVector2(topRightLocation.X / cellsSize.X, topRightLocation.Y / cellsSize.Y);
    topRightCoordinates = FIntVector2(FMath::Min(topRightCoordinates.X, numbersOfCells.X - 1), FMath::Min(topRightCoordinates.Y, numbersOfCells.Y - 1));

    check(bottomLeftCoordinates.X <= topRightCoordinates.X && bottomLeftCoordinates.Y <= topRightCoordinates.Y);

    const auto convert2DTo1D = [nRows = numbersOfCells.X](int32 i, int32 j) -> int32 { return j * nRows + i; };
    // The currDeltaT of the interactor takes all the interacting cells into account
    float interactorCurrDeltaT = 0.0f;
    int32 numberOfInteractingCells = 0;

    TArray<float>& currentTemperatures = _grid.CurrentTemperatures;
    TArray<float>& nextTemperatures = _grid.NextTemperatures;

    // Looping over cells intersecting the interactor's enclosing square (only these can intersect the interactor's circle)
    for (int32 j = bottomLeftCoordinates.Y; j <= topRightCoordinates.Y; ++j) {
        for (int32 i = bottomLeftCoordinates.X; i <= topRightCoordinates.X; ++i) {
            const int32 k = convert2DTo1D(i, j);
            if (_rectangleCircleIntersection(_grid.Locations[k], cellsExtent, interactorWorldLocation, interactorRange)) {
                ++numberOfInteractingCells;
                // This currDeltaT is from the interactor's POV, which is why the interactorTemperature is on the right hand side.
                // If the interactor's T is greater than the cell's, currDeltaT < 0 => the interactor emits heat (interactorCurrDeltaT decreases)
                // and the cell absorbs heat (nextTemperatures[k] increases).
                // If the interactor's T is smaller than the cell's, currDeltaT > 0 => the interactor absorbs heat (interactorCurrDeltaT increases)
                // and the cell emits heat (nextTemperatures[k] decreases).
                const float currDeltaT = currentTemperatures[k] - interactorTemperature;

                interactorCurrDeltaT += currDeltaT;
                nextTemperatures[k] -= (UThermodynamicsSubsystem::ROD_CONSTANT * currDeltaT * deltaTime / _grid.Attributes.HeatCapacity);
            }
        }
    }

    // The normalized version of the interactor's currDeltaT is an average of the interactions with the cells. This is important to avoid having the grid weight
    // way more than other bodies. This is realistic: sure, we are using a grid to represent the air of our map, but in the end, air is a single body.
    const float interactorCurrDeltaT_Normalized = interactorCurrDeltaT / numberOfInteractingCells;
    return interactorCurrDeltaT_Normalized;
}

void SelfInteract(const float deltaTime) {
    const FIntVector2& numbersOfCells = _grid.Attributes.NumbersOfCells;
    const auto convert2DTo1D = [nRows = numbersOfCells.X](int32 i, int32 j) -> int32 { return j * nRows + i; };

    TArray<float>& currentTemperatures = _grid.CurrentTemperatures;
    TArray<float>& nextTemperatures = _grid.NextTemperatures;
    for (int32 j = 0; j < numbersOfCells.Y; ++j) {
        for (int32 i = 0; i < numbersOfCells.X; ++i) {
            const int32 k = convert2DTo1D(i, j);

            // The cell k interacts with its neighbors
            float totalCurrDeltaT = 0.0;
            if (i != 0) {
                const int32 bottomK = convert2DTo1D(i - 1, j);
                // This currDeltaT is from the cell k's POV, which is why currentTemperatures[k] is on the right hand side:
                // If bottomK's T is greater than the k's, then k's totalCurrDeltaT increases (k absorbs heat from bottomK). Viceversa, totalCurrDeltaT
                // decreases (heat flows from k to bottomK). bottomK.
                totalCurrDeltaT += currentTemperatures[bottomK] - currentTemperatures[k];
            }
            if (j != 0) {
                const int32 leftK = convert2DTo1D(i, j - 1);
                // This currDeltaT is from the cell k's POV, which is why currentTemperatures[k] is on the right hand side:
                // If leftK's T is greater than the k's, then k's totalCurrDeltaT increases (k absorbs heat from leftK). Viceversa, totalCurrDeltaT
                // decreases (heat flows from k to leftK). leftK.
                totalCurrDeltaT += currentTemperatures[leftK] - currentTemperatures[k];
            }
            if (i != numbersOfCells.X - 1) {
                const int32 topK = convert2DTo1D(i + 1, j);
                // This currDeltaT is from the cell k's POV, which is why currentTemperatures[k] is on the right hand side:
                // If topK's T is greater than the k's, then k's totalCurrDeltaT increases (k absorbs heat from topK). Viceversa, totalCurrDeltaT
                // decreases (heat flows from k to topK). topK.
                totalCurrDeltaT += currentTemperatures[topK] - currentTemperatures[k];
            }
            if (j != numbersOfCells.Y - 1) {
                const int32 rightK = convert2DTo1D(i, j + 1);
                // This currDeltaT is from the cell k's POV, which is why currentTemperatures[k] is on the right hand side:
                // If rightK's T is greater than the k's, then k's totalCurrDeltaT increases (k absorbs heat from rightK). Viceversa, totalCurrDeltaT
                // decreases (heat flows from k to rightK). rightK.
                totalCurrDeltaT += currentTemperatures[rightK] - currentTemperatures[k];
            }

            const float totalDeltaT = (UThermodynamicsSubsystem::ROD_CONSTANT * totalCurrDeltaT * deltaTime) / _grid.Attributes.HeatCapacity;
            nextTemperatures[k] += totalDeltaT;
        }
    }

    // Now that we finished updating the nextTemperatures, we can say they are the current ones
    for (int i = 0; i < nextTemperatures.Num(); ++i) {
        currentTemperatures[i] = nextTemperatures[i];
    }
}

const TArray<float>& GetTemperatures() {
    return _grid.CurrentTemperatures;
}

void DrawDebugHeatmap(const UWorld* const world, const float height) {
    const auto extent3D = FVector(_grid.Attributes.CellsExtent, 100.0f);

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