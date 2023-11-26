// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapApplierComponent.h"

#include "Engine/LevelScriptActor.h"
#include "HeatmapPresenterComponent.h"

UHeatmapApplierComponent::UHeatmapApplierComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UHeatmapApplierComponent::BeginPlay() {
    Super::BeginPlay();

    ALevelScriptActor* const levelScript = GetWorld()->GetLevelScriptActor();
    if (UHeatmapPresenterComponent* const heatmapPresenter = levelScript->FindComponentByClass<UHeatmapPresenterComponent>(); heatmapPresenter) {


    } else {
        UE_LOG(LogTemp, Error, TEXT("No UHeatmapPresenterComponent found on Level Script"));
    }
}
