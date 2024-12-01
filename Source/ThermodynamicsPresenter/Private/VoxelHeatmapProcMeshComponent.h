// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "VoxelRender/VoxelProceduralMeshComponent.h"
#include "VoxelHeatmapProcMeshComponent.generated.h"

/**
 * \brief TODO
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Thermodynamics), meta = (BlueprintSpawnableComponent))
class UVoxelHeatmapProcMeshComponent : public UVoxelProceduralMeshComponent {
    GENERATED_BODY()

protected:
    void BeginPlay() override;

private:
    void _applyToStaticMesh(TWeakObjectPtr<UMaterialInstanceDynamic> heatmapMID);

    TWeakObjectPtr<AVoxelWorld> _ownerVoxelWorld;

    FDelegateHandle _handle_OnHeatmapMIDSet;
};
