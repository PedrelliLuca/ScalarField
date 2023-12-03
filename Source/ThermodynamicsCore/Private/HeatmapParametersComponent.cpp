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
        const FVector boxSize = boxC->GetUnscaledBoxExtent() * 2.0;

        const double sizeCellsX = boxSize.X / _numCellsX;
        const double sizeCellsY = boxSize.Y / _numCellsY;

        HeatmapGrid::FHeatmapParameters heatmapParameters;
        heatmapParameters.InitialTemperature = _initialTemperature;
        heatmapParameters.NCellsX = _numCellsX;
        heatmapParameters.NCellsY = _numCellsY;
        heatmapParameters.SizeCellsX = sizeCellsX;
        heatmapParameters.SizeCellsY = sizeCellsY;

        HeatmapGrid::Initialize(MoveTemp(heatmapParameters));
    } else {
        UE_LOG(LogTemp, Error, TEXT("No UBoxComponent on owner actor, cells' size cannot be set!"));
    }
}
