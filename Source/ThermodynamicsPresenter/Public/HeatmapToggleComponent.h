// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "HeatmapToggleComponent.generated.h"

/**
 * \brief Component that manages how the widgets behave with respect to each other
 */
UCLASS()
class THERMODYNAMICSPRESENTER_API UHeatmapToggleComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UHeatmapToggleComponent();

protected:
    void BeginPlay() override;

private:
};
