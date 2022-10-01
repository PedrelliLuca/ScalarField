// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThermodynamicComponent.h"

#include "ThermodynamicActor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SCALARFIELD_API AThermodynamicActor : public AActor {
	GENERATED_BODY()
	
public:
	AThermodynamicActor();

private:
	void _updateMaterialBasedOnTemperature(double temperature);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Thermodynamics, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> _staticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Thermodynamics, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UThermodynamicComponent> _thermodynamicC;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _materialInstance;
};
