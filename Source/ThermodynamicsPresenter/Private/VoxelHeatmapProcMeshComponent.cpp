// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelHeatmapProcMeshComponent.h"

#include "ThermodynamicsSubsystem.h"
#include "VoxelWorld.h"

void UVoxelHeatmapProcMeshComponent::BeginPlay() {
    Super::BeginPlay();

    _ownerVoxelWorld = Cast<AVoxelWorld>(GetOwner());

    if (_ownerVoxelWorld.IsValid()) {
        UThermodynamicsSubsystem* const thermoSubsys = GetWorld()->GetSubsystem<UThermodynamicsSubsystem>();
        TWeakObjectPtr<UMaterialInstanceDynamic> const heatmapMID = thermoSubsys->GetHeatmapMaterialInstance();
        if (heatmapMID.IsValid()) {
            _applyToStaticMesh(heatmapMID);
        } else {
            _handle_OnHeatmapMIDSet = thermoSubsys->OnHeatmapMIDSet.AddUObject(this, &UVoxelHeatmapProcMeshComponent::_applyToStaticMesh);
        }
    } else {
        UE_LOG(LogTemp, Error, TEXT(__FUNCTION__ ": Owner is not of type AVoxelWorld"));
    }
}

void UVoxelHeatmapProcMeshComponent::_applyToStaticMesh(TWeakObjectPtr<UMaterialInstanceDynamic> heatmapMID) {
    check(_ownerVoxelWorld.IsValid());
    _ownerVoxelWorld->VoxelMaterial = heatmapMID.Get();

    UThermodynamicsSubsystem* const thermoSubsys = GetWorld()->GetSubsystem<UThermodynamicsSubsystem>();
    thermoSubsys->OnHeatmapMIDSet.Remove(_handle_OnHeatmapMIDSet);

    UMaterialInterface* boh = GetMaterial(0);
}
