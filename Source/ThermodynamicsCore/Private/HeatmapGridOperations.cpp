// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapGridOperations.h"

#include "HeatmapGrid.h"

namespace HeatmapGrid {
FHeatmapGrid _grid;

void Initialize(FHeatmapParameters&& heatmapParams) {
    FHeatmapGrid grid;
    grid.Sizes = FIntVector2(heatmapParams.SizeCellsX, heatmapParams.SizeCellsY);

    _grid = MoveTemp(grid);
}
} // namespace HeatmapGrid
