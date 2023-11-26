// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "HeatmapPresenterComponent.generated.h"

/**
 * \brief TODO
 */
UCLASS(ClassGroup = (Thermodynamics))
class THERMODYNAMICSPRESENTER_API UHeatmapPresenterComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UHeatmapPresenterComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
    void BeginPlay() override;

private:
    FColor _generateColorFromValue(float val);

    UPROPERTY(EditAnywhere, Category = "Heatmap Parameters")
    int32 _numCellsX;
    UPROPERTY(EditAnywhere, Category = "Heatmap Parameters")
    int32 _numCellsY;

    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _frequencyPI = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _phasePI = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _amplitude = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _offset = 0.0f;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UMaterialInterface> _heatmapMaterial;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> _heatmapMID;

    UPROPERTY()
    TObjectPtr<UTexture2D> _heatmapTexture;

    float _timer = 0.0f;
};
