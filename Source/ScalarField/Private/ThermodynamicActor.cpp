// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicActor.h"

#include "ThermodynamicsSettings.h"

AThermodynamicActor::AThermodynamicActor() {
	PrimaryActorTick.bCanEverTick = true;

	const UThermodynamicsSettings* const thermodynamicsSettings = GetDefault<UThermodynamicsSettings>();

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(_staticMesh);
	_materialInstance = _staticMesh->CreateDynamicMaterialInstance(0, thermodynamicsSettings->GetThermodynamicsMaterial(), TEXT("Thermodynamics Material"));

	_thermodynamicC = CreateDefaultSubobject<UThermodynamicComponent>(TEXT("Thermodynamic Component"));
	_thermodynamicC->SetupAttachment(RootComponent);
	_thermodynamicC->OnTemperatureChanged.AddUObject(this, &AThermodynamicActor::_updateMaterialBasedOnTemperature);

	if (_materialInstance != nullptr) {
		_updateMaterialBasedOnTemperature(_thermodynamicC->GetTemperature());
	}
}

void AThermodynamicActor::_updateMaterialBasedOnTemperature(const double temperature) {
	if (_materialInstance == nullptr) {
		return;
	}

	const double lerpAlpha = FMath::Clamp(temperature / 1000., 0., 1.);
	_materialInstance->SetScalarParameterValue(TEXT("MyParam"), lerpAlpha);
}