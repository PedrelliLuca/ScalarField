// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"

#include "DecalHeatMap.generated.h"

/*!
 * \brief Actor to visualize the heat map to the player
 */
UCLASS(Blueprintable)
class ADecalHeatMap : public ADecalActor {
    GENERATED_BODY()

public:
    ADecalHeatMap();

    void Tick(const float deltaTime) override;

protected:
    void BeginPlay() override;

private:
    FColor _generateColorFromValue(float val) const;

    TWeakObjectPtr<UMaterialInstanceDynamic> _materialInstance;
    TWeakObjectPtr<UTexture2D> _materialTextureParam;
    
    UPROPERTY(EditAnywhere, Category = "Cells Parameters")
    int32 _width = 100;
    UPROPERTY(EditAnywhere, Category = "Cells Parameters")
    int32 _height = 100;

    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _frequencyPI = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _phasePI = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _amplitude = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Wave Parameters")
    float _offset = 0.0f;

    float _timer = 0.0f;
};
