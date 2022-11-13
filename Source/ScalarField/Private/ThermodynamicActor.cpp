// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicActor.h"

#include "Colorizer.h"
#include "ThermodynamicsSettings.h"

AThermodynamicActor::AThermodynamicActor() {
	PrimaryActorTick.bCanEverTick = true;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(_staticMesh);

	_thermodynamicCapsuleC = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Thermodynamic Capsule"));
	_thermodynamicCapsuleC->SetupAttachment(RootComponent);
	_thermodynamicCapsuleC->SetCollisionProfileName("HeatExchanger");

	_thermodynamicC = CreateDefaultSubobject<UNewThermodynamicComponent>(TEXT("Thermodynamic Component"));
}

void AThermodynamicActor::SetThermicCapsuleDimensions(const double radius, const double halfHeight) {
	_thermodynamicCapsuleC->SetCapsuleRadius(radius);
	_thermodynamicCapsuleC->SetCapsuleHalfHeight(halfHeight);
}

void AThermodynamicActor::BeginPlay() {
	Super::BeginPlay();
	_thermodynamicC->SetThermodynamicCollision(_thermodynamicCapsuleC);

	// Setting up the DMI that changes the mesh color based on temperature
	const UThermodynamicsSettings* const thermodynamicsSettings = GetDefault<UThermodynamicsSettings>();
	_materialInstance = _staticMesh->CreateDynamicMaterialInstance(0, thermodynamicsSettings->GetThermodynamicsMaterial(), TEXT("Thermodynamics Material"));

	if (_materialInstance != nullptr) {
		_updateMaterialBasedOnTemperature(_thermodynamicC->GetTemperature());
		_thermodynamicC->OnTemperatureChanged.AddUObject(this, &AThermodynamicActor::_updateMaterialBasedOnTemperature);
	}
}

void AThermodynamicActor::_updateMaterialBasedOnTemperature(const double temperature) {
	check(!_materialInstance.IsNull())
	_materialInstance->SetVectorParameterValue(TEXT("temperature"), FColorizer::GenerateColorFromTemperature(temperature));
}