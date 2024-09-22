// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapGridOperations.h"

#include "CollisionsCollectionComponent.h"
#include "DrawDebugHelpers.h"
#include "HeatmapGrid.h"
#include "ThermodynamicsSubsystem.h"

namespace HeatmapGrid {
TOptional<FHeatmapGrid> _grid;

void Initialize(FHeatmapParameters&& params) {
    FHeatmapGrid grid;
    FHeatmapGridAttributes attributes;
    attributes.CellsExtent = params.ExtentCells;
    attributes.NumbersOfCells = params.NumberCells;
    attributes.HeatCapacity = params.HeatCapacity;

    FVector2D const& gridLocation = params.LocationGrid;
    float const heatmapExtensionX = params.NumberCells.X * params.ExtentCells.X;
    float const heatmapExtensionY = params.NumberCells.Y * params.ExtentCells.Y;
    attributes.BottomLeftCorner = FVector2D(gridLocation.X - heatmapExtensionX, gridLocation.Y - heatmapExtensionY);
    auto const firstCellLocation = FVector2D(attributes.BottomLeftCorner.X + params.ExtentCells.X, attributes.BottomLeftCorner.Y + params.ExtentCells.Y);

    grid.Attributes = MoveTemp(attributes);

    int32 const totalNumberOfCells = params.NumberCells.X * params.NumberCells.Y;
    grid.Locations.Reserve(totalNumberOfCells);
    grid.CurrentTemperatures.Reserve(totalNumberOfCells);
    grid.NextTemperatures.Reserve(totalNumberOfCells);

    for (int32 j = 0; j < params.NumberCells.Y; ++j) {
        float const locationOffsetY = j * params.ExtentCells.Y * 2.0f;
        for (int32 i = 0; i < params.NumberCells.X; ++i) {
            float const locationOffsetX = i * params.ExtentCells.X * 2.0f;
            auto const locationOffset = FVector2D(locationOffsetX, locationOffsetY);
            grid.Locations.Emplace(firstCellLocation + locationOffset);
            grid.CurrentTemperatures.Emplace(params.InitialTemperature);
            grid.NextTemperatures.Emplace(params.InitialTemperature);
        }
    }

    _grid = MoveTemp(grid);
}

void Deinitialize() {
    _grid.Reset();
}

bool _doesCellIntersectCircle(FVector2D const cellLocation, FVector2D const cellExtension, FVector2D const circleLocation, float const radius) {
    auto const distance = FVector2D(FMath::Abs(circleLocation.X - cellLocation.X), FMath::Abs(circleLocation.Y - cellLocation.Y));

    if (distance.X > (cellExtension.X + radius) || distance.Y > (cellExtension.Y + radius)) {
        return false;
    }

    if (distance.X <= cellExtension.X || distance.Y <= cellExtension.Y) {
        return true;
    }

    // If you're here it means that:
    // cellExtension.X < distance.X < cellExtension.X + radius
    // cellExtension.Y < distance.Y < cellExtension.Y + radius
    double const cornerDistanceSquared = FMath::Pow(distance.X - cellExtension.X, 2.0) + FMath::Pow(distance.Y - cellExtension.Y, 2.0);
    return cornerDistanceSquared <= radius * radius;
}

bool _doesPolygonIntersectPolygon(TArray<FVector2D> const& polygonA, TArray<FVector2D> const& polygonB) {
    // If we can find a line separating the two polygons, they do not intersect. Let's look for it.

    // For each polygon...
    auto const polygons = TArray<TArray<FVector2D>>{polygonA, polygonB};
    for (auto const& polygon : polygons) {
        // For each edge (p1, p2)...
        for (int32 i1 = 0; i1 < polygon.Num(); ++i1) {
            int32 const i2 = (i1 + 1) % polygon.Num();
            FVector2D const& p1 = polygon[i1];
            FVector2D const& p2 = polygon[i2];

            // The edge is a separating line if, given any of its perpendicular axes, the projection of the two rectangles on that axes do not intersect.
            // For simplicity, we use the perpendicular axes passing by the origin (0, 0)
            auto const n = FVector2D(p2.Y - p1.Y, p1.X - p2.X);

            // We project polygonA on n by using the dot product. minA is the point of polyA whose distance from 0 is min, maxA is analogous.
            auto minA = TNumericLimits<double>::Max();
            auto maxA = TNumericLimits<double>::Lowest();
            for (auto const& pA : polygonA) {
                double const projected = FVector2D::DotProduct(n, pA);

                if (projected < minA) {
                    minA = projected;
                }
                if (projected > maxA) {
                    maxA = projected;
                }
            }

            // Repeat for polygon B
            auto minB = TNumericLimits<double>::Max();
            auto maxB = TNumericLimits<double>::Lowest();
            for (auto const& pB : polygonB) {
                double const projected = FVector2D::DotProduct(n, pB);

                if (projected < minB) {
                    minB = projected;
                }
                if (projected > maxB) {
                    maxB = projected;
                }
            }

            if (maxA < minB || maxB < minA) {
                // Yep, edge (p1, p2) is a separating line, the two polygons do not intersect.
                return false;
            }
        }
    }

    // We haven't found a single edge acting as separating line, meaning the polygons do intersect.
    return true;
}

bool _doesCellIntersectRectangle(FVector2D const cellLocation, FVector2D const cellExtension, TArray<FVector2D> const& rectangleVertices) {
    auto cellVertices = TArray<FVector2D>();
    cellVertices.Reserve(4);
    cellVertices.Emplace(cellLocation.X - cellExtension.X, cellLocation.Y - cellExtension.Y);
    cellVertices.Emplace(cellLocation.X - cellExtension.X, cellLocation.Y + cellExtension.Y);
    cellVertices.Emplace(cellLocation.X + cellExtension.X, cellLocation.Y + cellExtension.Y);
    cellVertices.Emplace(cellLocation.X + cellExtension.X, cellLocation.Y - cellExtension.Y);
    return _doesPolygonIntersectPolygon(cellVertices, rectangleVertices);
}

void _spheresInteraction(FHeatmapGrid& grid, TArray<bool>& didInteractWithCell, int32& numberOfInteractingCells, float& collectionCurrDeltaT,
    FTransform const& interactorTransform, TArray<FCollectionSphereParameters> const& spheres, float const collectionTemperature, float const deltaTime) {
    FVector2D const& cellsExtent = grid.Attributes.CellsExtent;
    auto const cellsSize = cellsExtent * 2.0;
    FIntVector2 const& numbersOfCells = grid.Attributes.NumbersOfCells;

    TArray<float>& currentTemperatures = grid.CurrentTemperatures;
    TArray<float>& nextTemperatures = grid.NextTemperatures;

    // We make each sphere of the collection interact with the grid
    for (FCollectionSphereParameters const& sphere : spheres) {
        // From Sphere Space to World Space to Grid Space
        auto const sphereToWorldTransform = sphere.RootRelativeTransform * interactorTransform;
        auto const sphereWorldLocation = FVector2D(sphereToWorldTransform.TransformPosition(FVector::ZeroVector));
        FVector2D const sphereGridLocation = sphereWorldLocation - grid.Attributes.BottomLeftCorner;
        bool const sphereWithinXBounds = 0.0f <= sphereGridLocation.X && sphereGridLocation.X <= numbersOfCells.X * cellsSize.X;
        bool const sphereWithinYBounds = 0.0f <= sphereGridLocation.Y && sphereGridLocation.Y <= numbersOfCells.Y * cellsSize.Y;
        // Is the sphere centre within the grid bounds?
        if (sphereWithinXBounds && sphereWithinYBounds) {
            // The cell (i, j) containing the bottom-left corner of the square enclosing the interaction circle
            FVector2D const bottomLeftLocation = sphereGridLocation - sphere.Radius * FVector2D::UnitVector;
            auto bottomLeftCoordinates = FIntVector2(bottomLeftLocation.X / cellsSize.X, bottomLeftLocation.Y / cellsSize.Y);
            bottomLeftCoordinates = FIntVector2(FMath::Max(bottomLeftCoordinates.X, 0), FMath::Max(bottomLeftCoordinates.Y, 0));

            // The cell (i, j) containing the top-right corner of the square enclosing the interaction circle
            FVector2D const topRightLocation = sphereGridLocation + sphere.Radius * FVector2D::UnitVector;
            auto topRightCoordinates = FIntVector2(topRightLocation.X / cellsSize.X, topRightLocation.Y / cellsSize.Y);
            topRightCoordinates = FIntVector2(FMath::Min(topRightCoordinates.X, numbersOfCells.X - 1), FMath::Min(topRightCoordinates.Y, numbersOfCells.Y - 1));

            check(bottomLeftCoordinates.X <= topRightCoordinates.X && bottomLeftCoordinates.Y <= topRightCoordinates.Y);

            auto const convert2DTo1D = [nRows = numbersOfCells.X](int32 i, int32 j) -> int32 { return j * nRows + i; };

            // Looping over cells intersecting the sphere's enclosing square (only these can intersect the interactor's circle)
            for (int32 j = bottomLeftCoordinates.Y; j <= topRightCoordinates.Y; ++j) {
                for (int32 i = bottomLeftCoordinates.X; i <= topRightCoordinates.X; ++i) {
                    int32 const k = convert2DTo1D(i, j);
                    if (!didInteractWithCell[k] && _doesCellIntersectCircle(grid.Locations[k], cellsExtent, sphereWorldLocation, sphere.Radius)) {
                        // This currDeltaT is from the collection's POV, which is why the collectionTemperature is on the right hand side.
                        // If the colletion's T is greater than the cell's, currDeltaT < 0 => the collection emits heat (collectionCurrDeltaT decreases)
                        // and the cell absorbs heat (nextTemperatures[k] increases).
                        // If the collection's T is smaller than the cell's, currDeltaT > 0 => the collection absorbs heat (collectionCurrDeltaT increases)
                        // and the cell emits heat (nextTemperatures[k] decreases).
                        float const currDeltaT = currentTemperatures[k] - collectionTemperature;

                        collectionCurrDeltaT += currDeltaT;
                        nextTemperatures[k] -= (UThermodynamicsSubsystem::ROD_CONSTANT * currDeltaT * deltaTime / grid.Attributes.HeatCapacity);

                        ++numberOfInteractingCells;
                        didInteractWithCell[k] = true;
                    }
                }
            }
        }
    }
}

void _boxSideInteraction(FHeatmapGrid& grid, TArray<bool>& didInteractWithCell, int32& numberOfInteractingCells, float& collectionCurrDeltaT,
    TStaticArray<FVector, 4> const& boxSide, FTransform const& boxToGridTransform, FTransform const& boxToWorldTransform, float const collectionTemperature,
    float const deltaTime) {
    FVector2D const& cellsExtent = grid.Attributes.CellsExtent;
    auto const cellsSize = cellsExtent * 2.0;
    FIntVector2 const& numbersOfCells = grid.Attributes.NumbersOfCells;

    TArray<float>& currentTemperatures = grid.CurrentTemperatures;
    TArray<float>& nextTemperatures = grid.NextTemperatures;

    TArray<FVector2D> boxGridVertices;
    boxGridVertices.Reserve(4);
    boxGridVertices.Emplace(boxToGridTransform.TransformPosition(boxSide[0]));
    boxGridVertices.Emplace(boxToGridTransform.TransformPosition(boxSide[1]));
    boxGridVertices.Emplace(boxToGridTransform.TransformPosition(boxSide[2]));
    boxGridVertices.Emplace(boxToGridTransform.TransformPosition(boxSide[3]));

    auto bottomLeftLocation = FVector2D(TNumericLimits<double>::Max(), TNumericLimits<double>::Max());
    auto topRightLocation = FVector2D(TNumericLimits<double>::Lowest(), TNumericLimits<double>::Lowest());

    for (FVector2D const& vertex : boxGridVertices) {
        bottomLeftLocation.X = FMath::Min(bottomLeftLocation.X, vertex.X);
        bottomLeftLocation.Y = FMath::Min(bottomLeftLocation.Y, vertex.Y);
        topRightLocation.X = FMath::Max(topRightLocation.X, vertex.X);
        topRightLocation.Y = FMath::Max(topRightLocation.Y, vertex.Y);
    }

    // The cell (i, j) containing the bottom-left corner of the box
    auto bottomLeftCoordinates = FIntVector2(bottomLeftLocation.X / cellsSize.X, bottomLeftLocation.Y / cellsSize.Y);
    bottomLeftCoordinates = FIntVector2(FMath::Max(bottomLeftCoordinates.X, 0), FMath::Max(bottomLeftCoordinates.Y, 0));

    // The cell (i, j) containing the top-right corner of the box
    auto topRightCoordinates = FIntVector2(topRightLocation.X / cellsSize.X, topRightLocation.Y / cellsSize.Y);
    topRightCoordinates = FIntVector2(FMath::Min(topRightCoordinates.X, numbersOfCells.X - 1), FMath::Min(topRightCoordinates.Y, numbersOfCells.Y - 1));

    check(bottomLeftCoordinates.X <= topRightCoordinates.X && bottomLeftCoordinates.Y <= topRightCoordinates.Y);

    auto const convert2DTo1D = [nRows = numbersOfCells.X](int32 i, int32 j) -> int32 { return j * nRows + i; };

    TArray<FVector2D> boxWorldVertices;
    boxWorldVertices.Reserve(4);
    boxWorldVertices.Emplace(boxToWorldTransform.TransformPosition(boxSide[0]));
    boxWorldVertices.Emplace(boxToWorldTransform.TransformPosition(boxSide[1]));
    boxWorldVertices.Emplace(boxToWorldTransform.TransformPosition(boxSide[2]));
    boxWorldVertices.Emplace(boxToWorldTransform.TransformPosition(boxSide[3]));

    // Looping over cells intersecting the box's enclosing rectangle (only these can intersect the interactor's circle)
    for (int32 j = bottomLeftCoordinates.Y; j <= topRightCoordinates.Y; ++j) {
        for (int32 i = bottomLeftCoordinates.X; i <= topRightCoordinates.X; ++i) {
            int32 const k = convert2DTo1D(i, j);
            if (!didInteractWithCell[k] && _doesCellIntersectRectangle(grid.Locations[k], cellsExtent, boxWorldVertices)) {
                // This currDeltaT is from the collection's POV, which is why the collectionTemperature is on the right hand side.
                // If the colletion's T is greater than the cell's, currDeltaT < 0 => the collection emits heat (collectionCurrDeltaT decreases)
                // and the cell absorbs heat (nextTemperatures[k] increases).
                // If the collection's T is smaller than the cell's, currDeltaT > 0 => the collection absorbs heat (collectionCurrDeltaT increases)
                // and the cell emits heat (nextTemperatures[k] decreases).
                float const currDeltaT = currentTemperatures[k] - collectionTemperature;

                collectionCurrDeltaT += currDeltaT;
                nextTemperatures[k] -= (UThermodynamicsSubsystem::ROD_CONSTANT * currDeltaT * deltaTime / grid.Attributes.HeatCapacity);

                ++numberOfInteractingCells;
                didInteractWithCell[k] = true;
            }
        }
    }
}

void _boxesInteraction(FHeatmapGrid& grid, TArray<bool>& didInteractWithCell, int32& numberOfInteractingCells, float& collectionCurrDeltaT,
    FTransform const& interactorTransform, TArray<FCollectionBoxParameters> const& boxes, float const collectionTemperature, float const deltaTime) {
    auto const cellsSize = grid.Attributes.CellsExtent * 2.0;
    FIntVector2 const& numbersOfCells = grid.Attributes.NumbersOfCells;

    auto const worldToGridTransform = FTransform(FVector(grid.Attributes.BottomLeftCorner, 0.0f)).Inverse();

    // We make each sphere of the collection interact with the grid
    for (FCollectionBoxParameters const& box : boxes) {
        // From Box Space to World Space to Grid Space
        auto const boxToWorldTransform = box.RootRelativeTransform * interactorTransform;
        auto const boxToGridTransform = boxToWorldTransform * worldToGridTransform;
        auto const boxGridLocation = FVector2D(boxToGridTransform.TransformPosition(FVector::ZeroVector));
        bool const sphereWithinXBounds = 0.0f <= boxGridLocation.X && boxGridLocation.X <= numbersOfCells.X * cellsSize.X;
        bool const sphereWithinYBounds = 0.0f <= boxGridLocation.Y && boxGridLocation.Y <= numbersOfCells.Y * cellsSize.Y;
        // Is the box centre within the grid bounds?
        if (sphereWithinXBounds && sphereWithinYBounds) {
            _boxSideInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, box.FrontFace, boxToGridTransform,
                boxToWorldTransform, collectionTemperature, deltaTime);
            _boxSideInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, box.LeftFace, boxToGridTransform,
                boxToWorldTransform, collectionTemperature, deltaTime);
            _boxSideInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, box.BackFace, boxToGridTransform,
                boxToWorldTransform, collectionTemperature, deltaTime);
            _boxSideInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, box.RightFace, boxToGridTransform,
                boxToWorldTransform, collectionTemperature, deltaTime);
            _boxSideInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, box.TopFace, boxToGridTransform, boxToWorldTransform,
                collectionTemperature, deltaTime);
            _boxSideInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, box.BottomFace, boxToGridTransform,
                boxToWorldTransform, collectionTemperature, deltaTime);
        }
    }
}

void _capsulesInteraction(FHeatmapGrid& grid, TArray<bool>& didInteractWithCell, int32& numberOfInteractingCells, float& collectionCurrDeltaT,
    FTransform const& interactorTransform, TArray<FCollectionCapsuleParameters> const& capsules, float const collectionTemperature, float const deltaTime) {
    for (FCollectionCapsuleParameters const& capsule : capsules) {
        TArray<FCollectionBoxParameters> capsuleBox;
        capsuleBox.Add(capsule.CylinderBox);
        _boxesInteraction(
            grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, interactorTransform, capsuleBox, collectionTemperature, deltaTime);

        TArray<FCollectionSphereParameters> capsuleSpheres;
        capsuleSpheres.Add(capsule.UpperSphere);
        capsuleSpheres.Add(capsule.LowerSphere);
        _spheresInteraction(
            grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, interactorTransform, capsuleSpheres, collectionTemperature, deltaTime);
    }
}

float Interact(
    FTransform const& interactorTransform, UCollisionsCollectionComponent* collisionsCollection, float const collectionTemperature, float const deltaTime) {
    float collectionCurrDeltaT_Normalized = 0.0f;
    if (_grid.IsSet()) {
        check(_grid.IsSet());

        FHeatmapGrid& grid = _grid.GetValue();

        // Given a cell, we don't want it to interact with more than one element of the collection, as this latter is meant to represent a single body. This
        // cache makes it possible to avoid that.
        TArray<bool> didInteractWithCell;
        didInteractWithCell.Reserve(grid.Locations.Num());
        didInteractWithCell.Init(false, grid.Locations.Num());

        // The currDeltaT of the collection takes all the interacting cells into account
        float collectionCurrDeltaT = 0.0f;
        int32 numberOfInteractingCells = 0;

        _spheresInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, interactorTransform,
            collisionsCollection->GetCollectionSpheres(), collectionTemperature, deltaTime);
        _boxesInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, interactorTransform,
            collisionsCollection->GetCollectionBoxes(), collectionTemperature, deltaTime);
        _capsulesInteraction(grid, didInteractWithCell, numberOfInteractingCells, collectionCurrDeltaT, interactorTransform,
            collisionsCollection->GetCollectionCapsules(), collectionTemperature, deltaTime);

        // The normalized version of the collection's currDeltaT is an average of the interactions with the cells. This is important to avoid having the
        // grid weight way more than other collections (i.e. other bodies). This is realistic: sure, we are using a grid to represent the air of our map,
        // but in the end air is a single body.
        collectionCurrDeltaT_Normalized = collectionCurrDeltaT / numberOfInteractingCells;
    }

    return collectionCurrDeltaT_Normalized;
}

void SelfInteract(float const deltaTime) {
    if (_grid.IsSet()) {
        FHeatmapGrid& grid = _grid.GetValue();
        FIntVector2 const& numbersOfCells = grid.Attributes.NumbersOfCells;
        auto const convert2DTo1D = [nRows = numbersOfCells.X](int32 i, int32 j) -> int32 { return j * nRows + i; };

        TArray<float>& currentTemperatures = grid.CurrentTemperatures;
        TArray<float>& nextTemperatures = grid.NextTemperatures;
        for (int32 j = 0; j < numbersOfCells.Y; ++j) {
            for (int32 i = 0; i < numbersOfCells.X; ++i) {
                int32 const k = convert2DTo1D(i, j);

                // The cell k interacts with its neighbors
                float totalCurrDeltaT = 0.0;
                if (i != 0) {
                    int32 const bottomK = convert2DTo1D(i - 1, j);
                    // This currDeltaT is from the cell k's POV, which is why currentTemperatures[k] is on the right hand side:
                    // If bottomK's T is greater than the k's, then k's totalCurrDeltaT increases (k absorbs heat from bottomK). Viceversa, totalCurrDeltaT
                    // decreases (heat flows from k to bottomK). bottomK.
                    totalCurrDeltaT += currentTemperatures[bottomK] - currentTemperatures[k];
                }
                if (j != 0) {
                    int32 const leftK = convert2DTo1D(i, j - 1);
                    // This currDeltaT is from the cell k's POV, which is why currentTemperatures[k] is on the right hand side:
                    // If leftK's T is greater than the k's, then k's totalCurrDeltaT increases (k absorbs heat from leftK). Viceversa, totalCurrDeltaT
                    // decreases (heat flows from k to leftK). leftK.
                    totalCurrDeltaT += currentTemperatures[leftK] - currentTemperatures[k];
                }
                if (i != numbersOfCells.X - 1) {
                    int32 const topK = convert2DTo1D(i + 1, j);
                    // This currDeltaT is from the cell k's POV, which is why currentTemperatures[k] is on the right hand side:
                    // If topK's T is greater than the k's, then k's totalCurrDeltaT increases (k absorbs heat from topK). Viceversa, totalCurrDeltaT
                    // decreases (heat flows from k to topK). topK.
                    totalCurrDeltaT += currentTemperatures[topK] - currentTemperatures[k];
                }
                if (j != numbersOfCells.Y - 1) {
                    int32 const rightK = convert2DTo1D(i, j + 1);
                    // This currDeltaT is from the cell k's POV, which is why currentTemperatures[k] is on the right hand side:
                    // If rightK's T is greater than the k's, then k's totalCurrDeltaT increases (k absorbs heat from rightK). Viceversa, totalCurrDeltaT
                    // decreases (heat flows from k to rightK). rightK.
                    totalCurrDeltaT += currentTemperatures[rightK] - currentTemperatures[k];
                }

                float const totalDeltaT = (UThermodynamicsSubsystem::ROD_CONSTANT * totalCurrDeltaT * deltaTime) / grid.Attributes.HeatCapacity;
                nextTemperatures[k] += totalDeltaT;
            }
        }

        // Now that we finished updating the nextTemperatures, we can say they are the current ones
        for (int i = 0; i < nextTemperatures.Num(); ++i) {
            currentTemperatures[i] = nextTemperatures[i];
        }
    }
}

TArray<float> const& GetTemperatures() {
    check(_grid.IsSet());
    return _grid->CurrentTemperatures;
}

void DrawDebugHeatmap(UWorld const* const world, float const height) {
    if (_grid.IsSet()) {
        FHeatmapGrid& grid = _grid.GetValue();
        auto const extent3D = FVector(grid.Attributes.CellsExtent, 100.0f);

        int32 const nCells = grid.Locations.Num();
        float const colorStep = 255.0f / nCells;
        auto color = FColor::Red;

        for (FVector2D const& location : grid.Locations) {
            auto const location3D = FVector(location, height);
            DrawDebugBox(world, location3D, extent3D, color, false, 2.0f);

            // TODO: this is broken, as increments < 1 cannot be appreciated. You should use a color map
            // similar to the one in HeatmapPresenterComponent.cpp. This function should be in a new debug
            // component in ThermodynamicsPresenter or, at the very least, within HeatmapPresenterComponent.
            color.R -= colorStep;
            color.B += colorStep;
        }
    }
}

} // namespace HeatmapGrid
