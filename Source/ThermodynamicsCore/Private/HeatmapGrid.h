// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace HeatmapGrid {

struct FHeatmapGridAttributes {
    FVector2D CellsExtent;
    FIntVector2 NumbersOfCells;
    FVector2D BottomLeftCorner;
    float HeatCapacity;
};

//  1D Grid Representation
//
//  +----+----+----+----+----+----+----+----+----+
//  | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  |
//  |    |    |    |    |    |    |    |    |    |
//  +----+----+----+----+----+----+----+----+----+
//  ----------------------------------------------->
//                                                 I
//  2D Grid Representation
//
//  X ^
//    |
//    |   +----+----+----+
//  2 |   | 2  | 5  | 8  |
//    |   |    |    |    |
//    |   +----|----|----+
//  1 |   | 1  | 4  | 7  |
//    |   |    |    |    |
//    |   +----|----|----+
//  0 |   | 0  | 3  | 6  |
//    |   |    |    |    |
//    |   +----+----+----+
//    +---------------------->
//          0    1    2      Y
//
// 2D to 1D representation formula:
// I = Y * nX + X
//
// Example (1,2) => I = 2 * 3 + 1 = 7
struct FHeatmapGrid {
    // Collection of "cold" data, i.e. data that isn't looped on.
    FHeatmapGridAttributes Attributes;

    TArray<FVector2D> Locations;

    // Double Buffer pattern to ensure the entire heatmap gets updated at once.
    TArray<float> CurrentTemperatures;
    TArray<float> NextTemperatures;
};
} // namespace HeatmapGrid
