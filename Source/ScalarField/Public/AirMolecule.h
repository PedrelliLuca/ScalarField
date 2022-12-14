// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "ThermodynamicActor.h"

#include "AirMolecule.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SCALARFIELD_API AAirMolecule : public AThermodynamicActor {
	GENERATED_BODY()

public:
	AAirMolecule();
	void SetThermalCapsuleDimensions(const double radius, const double halfHeight);

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UCapsuleComponent> _capsuleThermalCollision;
};
