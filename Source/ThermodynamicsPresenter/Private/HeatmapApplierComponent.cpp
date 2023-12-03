// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapApplierComponent.h"

#include "HeatmapPresenterComponent.h"
#include "ThermodynamicsSubsystem.h"

UHeatmapApplierComponent::UHeatmapApplierComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UHeatmapApplierComponent::BeginPlay() {
    Super::BeginPlay();

    UThermodynamicsSubsystem* thermoSubsys = GetWorld()->GetSubsystem<UThermodynamicsSubsystem>();
    const TWeakObjectPtr<UMaterialInstanceDynamic> heatmapMID = thermoSubsys->GetHeatmapMaterialInstance();
    if (heatmapMID.IsValid()) {
        _applyToStaticMesh(heatmapMID);
    } else {
        _handle_OnHeatmapMIDSet = thermoSubsys->OnHeatmapMIDSet.AddUObject(this, &UHeatmapApplierComponent::_applyToStaticMesh);
    }
}

void UHeatmapApplierComponent::_applyToStaticMesh(const TWeakObjectPtr<UMaterialInstanceDynamic> heatmapMID) {
    UStaticMeshComponent* const staticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    staticMesh->SetMaterial(0, heatmapMID.Get());

    UThermodynamicsSubsystem* thermoSubsys = GetWorld()->GetSubsystem<UThermodynamicsSubsystem>();
    thermoSubsys->OnHeatmapMIDSet.Remove(_handle_OnHeatmapMIDSet);
}
