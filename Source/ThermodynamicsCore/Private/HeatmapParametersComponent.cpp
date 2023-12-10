// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapParametersComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HeatmapGridOperations.h"
#include "HeatmapParametersComponent.h"

UHeatmapParametersComponent::UHeatmapParametersComponent()
    : _numCellsX(0)
    , _numCellsY(0)
    , _initialTemperature(273.0f) {
    PrimaryComponentTick.bCanEverTick = false;
}

FIntVector2 UHeatmapParametersComponent::GetNumberOfCellsXY() const {
    return FIntVector2(_numCellsX, _numCellsY);
}

void UHeatmapParametersComponent::BeginPlay() {
    Super::BeginPlay();

    if (UBoxComponent* const boxC = GetOwner()->FindComponentByClass<UBoxComponent>(); boxC) {
        const FVector boxExtent = boxC->GetUnscaledBoxExtent();

        const auto extentCellsX = static_cast<float>(boxExtent.X / _numCellsX);
        const auto extentCellsY = static_cast<float>(boxExtent.Y / _numCellsY);

        HeatmapGrid::FHeatmapParameters heatmapParameters;
        heatmapParameters.LocationGrid = FVector2D(boxC->GetComponentTransform().GetLocation());
        heatmapParameters.ExtentCells = FVector2D(extentCellsX, extentCellsY);
        heatmapParameters.NumberCells = FIntVector2(_numCellsX, _numCellsY);
        heatmapParameters.InitialTemperature = _initialTemperature;

        HeatmapGrid::Initialize(MoveTemp(heatmapParameters));
        HeatmapGrid::DrawDebugHeatmap(GetWorld(), 1000.0f);
    } else {
        UE_LOG(LogTemp, Error, TEXT("No UBoxComponent on owner actor, cells' size cannot be set!"));
    }
}
