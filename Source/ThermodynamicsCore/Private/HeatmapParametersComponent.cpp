// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapParametersComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HeatmapGridOperations.h"

UHeatmapParametersComponent::UHeatmapParametersComponent()
    : _numCellsX(0)
    , _numCellsY(0)
    , _initialTemperature(273.0f)
    , _heatCapacity(1.0f) {
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = ETickingGroup::TG_DuringPhysics;
}

FIntVector2 UHeatmapParametersComponent::GetNumberOfCellsXY() const {
    return FIntVector2(_numCellsX, _numCellsY);
}

void UHeatmapParametersComponent::TickComponent(float const deltaTime, ELevelTick const tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    HeatmapGrid::SelfInteract(deltaTime);
}

void UHeatmapParametersComponent::BeginDestroy() {
    Super::BeginDestroy();

    HeatmapGrid::Deinitialize();
}

void UHeatmapParametersComponent::BeginPlay() {
    Super::BeginPlay();

    if (UBoxComponent* const boxC = GetOwner()->FindComponentByClass<UBoxComponent>(); boxC) {
        FVector const boxExtent = boxC->GetUnscaledBoxExtent();

        auto const extentCellsX = static_cast<float>(boxExtent.X / _numCellsX);
        auto const extentCellsY = static_cast<float>(boxExtent.Y / _numCellsY);

        HeatmapGrid::FHeatmapParameters heatmapParameters;
        heatmapParameters.LocationGrid = FVector2D(boxC->GetComponentTransform().GetLocation());
        heatmapParameters.ExtentCells = FVector2D(extentCellsX, extentCellsY);
        heatmapParameters.NumberCells = FIntVector2(_numCellsX, _numCellsY);
        heatmapParameters.InitialTemperature = _initialTemperature;
        heatmapParameters.HeatCapacity = _heatCapacity;

        HeatmapGrid::Initialize(MoveTemp(heatmapParameters));
    } else {
        UE_LOG(LogTemp, Error, TEXT("No UBoxComponent on owner actor, cells' size cannot be set!"));
    }
}
