// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermodynamicsSubsystem.h"

void UThermodynamicsSubsystem::SetHeatmapMaterialInstance(TObjectPtr<UMaterialInstanceDynamic> heatmapMID) {
    check(heatmapMID);
    _heatmapMID = heatmapMID;
    OnHeatmapMIDSet.Broadcast(_heatmapMID);
}

TWeakObjectPtr<UMaterialInstanceDynamic> UThermodynamicsSubsystem::GetHeatmapMaterialInstance() const {
    return _heatmapMID.Get();
}
