// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "ThermodynamicsSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHeatmapMIDSet, TWeakObjectPtr<UMaterialInstanceDynamic>);
DECLARE_MULTICAST_DELEGATE(FOnHeatmapVisualizationToggle);

/*!
 * \brief TODO
 */
UCLASS()
class THERMODYNAMICSCORE_API UThermodynamicsSubsystem : public UWorldSubsystem {
    GENERATED_BODY()

public:
    void SetHeatmapMaterialInstance(TObjectPtr<UMaterialInstanceDynamic> heatmapMID);
    TWeakObjectPtr<UMaterialInstanceDynamic> GetHeatmapMaterialInstance() const;

    FOnHeatmapMIDSet OnHeatmapMIDSet;
    FOnHeatmapVisualizationToggle OnHeatmapVisualizationToggle;

private:
    TWeakObjectPtr<UMaterialInstanceDynamic> _heatmapMID;
};
