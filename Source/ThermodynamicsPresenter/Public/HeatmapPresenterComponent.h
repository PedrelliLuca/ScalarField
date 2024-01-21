// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "HeatmapPresenterComponent.generated.h"

class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;

/**
 * \brief TODO
 */
UCLASS(ClassGroup = (Thermodynamics), meta = (BlueprintSpawnableComponent))
class THERMODYNAMICSPRESENTER_API UHeatmapPresenterComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UHeatmapPresenterComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
    void BeginPlay() override;

private:
    void _updateHeatmap();

    void _toggleHeatmapVisualization();

    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _frequencyPI = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _phasePI = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _amplitude = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _offset = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Material")
    TObjectPtr<UMaterialInterface> _heatmapMaterial;

    UPROPERTY(EditAnywhere, Category = "Material Parameter Collection")
    TObjectPtr<UMaterialParameterCollection> _heatmapMPC;

    UPROPERTY(EditAnywhere, Category = "Material Parameter Collection")
    FName _toggleParameterName = "";

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> _heatmapMID;

    UPROPERTY()
    TObjectPtr<UTexture2D> _heatmapTexture;

    TWeakObjectPtr<UMaterialParameterCollectionInstance> _heatmapMPCI;

    float _timer = 0.0f;
    int32 _nPixels;
    bool _isHeatmapVisible = true;
};
