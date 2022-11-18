// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
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
	void SetThermicCapsuleDimensions(double radius, double halfHeight);

protected:
	void BeginPlay() override;

private:
	void _updateMaterialBasedOnTemperature(double temperature);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> _staticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thermodynamics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> _simpleThermodynamicCollision;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> _complexThermoCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thermodynamics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNewThermodynamicComponent> _thermodynamicC;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _materialInstance;
};
