// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeatMap.h"

AHeatMap::AHeatMap() {
    _staticMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    SetRootComponent(_staticMeshC);

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void AHeatMap::Tick(const float deltaTime) {
    Super::Tick(deltaTime);

    if (_materialInstance.IsValid())
    {
        const float frequency = _frequencyPI * PI;
        const float phase = _phasePI * PI;
        const float waveValue = _offset + _amplitude * FMath::Sin(phase + frequency * _timer);
        _materialInstance->SetScalarParameterValue(FName("Alpha"), waveValue);

        _timer += deltaTime;
    }
}

void AHeatMap::BeginPlay() {
    Super::BeginPlay();

    _materialInstance = _staticMeshC->CreateDynamicMaterialInstance(0, _material, FName(""));
}
