// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "HeatmapParametersComponent.generated.h"

/**
 * \brief TODO
 */
UCLASS(ClassGroup = (Thermodynamics), meta = (BlueprintSpawnableComponent))
class THERMODYNAMICSCORE_API UHeatmapParametersComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UHeatmapParametersComponent();

    FIntVector2 GetNumberOfCellsXY() const;

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
    void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Heatmap Parameters")
    int32 _numCellsX;
    UPROPERTY(EditAnywhere, Category = "Heatmap Parameters")
    int32 _numCellsY;
    UPROPERTY(EditAnywhere, Category = "Heatmap Parameters")
    float _initialTemperature;
    UPROPERTY(EditAnywhere, Category = "Heatmap Parameters")
    float _heatCapacity;
};
