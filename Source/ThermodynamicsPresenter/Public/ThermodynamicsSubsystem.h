// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "ThermodynamicsSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHeatmapMIDSet, TWeakObjectPtr<UMaterialInstanceDynamic>);

/*!
 * \brief TODO
 */
UCLASS()
class THERMODYNAMICSPRESENTER_API UThermodynamicsSubsystem : public UWorldSubsystem {
    GENERATED_BODY()

public:
    void SetHeatmapMaterialInstance(TObjectPtr<UMaterialInstanceDynamic> heatmapMID);
    TWeakObjectPtr<UMaterialInstanceDynamic> GetHeatmapMaterialInstance() const;

    FOnHeatmapMIDSet OnHeatmapMIDSet;

private:
    TWeakObjectPtr<UMaterialInstanceDynamic> _heatmapMID;
};
