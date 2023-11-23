// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapToggleComponent.h"

UHeatmapToggleComponent::UHeatmapToggleComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UHeatmapToggleComponent::BeginPlay() {
    Super::BeginPlay();
}
