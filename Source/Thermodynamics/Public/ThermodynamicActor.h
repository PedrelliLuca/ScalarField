// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PrimitiveComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThermodynamicComponent.h"

#include "ThermodynamicActor.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class THERMODYNAMICS_API AThermodynamicActor : public AActor {
    GENERATED_BODY()

public:
    AThermodynamicActor();

    void SetTemperature(double temperature, bool updateInitialTemperature = false) { _thermodynamicC->SetTemperature(temperature, updateInitialTemperature); }
    void SetHeatCapacity(double heatCapacity) { _thermodynamicC->SetHeatCapacity(heatCapacity); }

protected:
    void BeginPlay() override;

    TObjectPtr<const UThermodynamicComponent> _getThermodynamicComponent() const;

private:
    void _setupThermodynamicCollisions();
    void _updateMaterialBasedOnTemperature(double temperature);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UStaticMeshComponent> _staticMesh;

    UPROPERTY()
    TObjectPtr<UPrimitiveComponent> _simpleThermalCollision = nullptr;

    UPROPERTY()
    TObjectPtr<UPrimitiveComponent> _complexThermalCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thermodynamics", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UThermodynamicComponent> _thermodynamicC;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> _materialInstance;
};
