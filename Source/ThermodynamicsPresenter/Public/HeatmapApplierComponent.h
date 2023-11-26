// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "HeatmapApplierComponent.generated.h"

/**
 * \brief TODO
 */
UCLASS()
class THERMODYNAMICSPRESENTER_API UHeatmapApplierComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UHeatmapApplierComponent();

protected:
    void BeginPlay() override;

private:
};
