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

    // This is the factor (k*A)/L in the heat exchange formulas (4) and (5). Tune this if you want to change the speed of all thermodyamic interactions in the
    // game.
    static constexpr float ROD_CONSTANT = 1.0f;

private:
    TWeakObjectPtr<UMaterialInstanceDynamic> _heatmapMID;
};
