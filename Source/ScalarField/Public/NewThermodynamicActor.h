// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PrimitiveComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewThermodynamicComponent.h"

#include "NewThermodynamicActor.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SCALARFIELD_API ANewThermodynamicActor : public AActor {
	GENERATED_BODY()

public:
	ANewThermodynamicActor();

	void SetTemperature(double temperature, bool updateInitialTemperature = false) { _thermodynamicC->SetTemperature(temperature, updateInitialTemperature); }
	void SetHeatCapacity(double heatCapacity) { _thermodynamicC->SetHeatCapacity(heatCapacity); }

protected:
	void BeginPlay() override;

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
	TObjectPtr<UNewThermodynamicComponent> _thermodynamicC;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _materialInstance;
};
