// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicActor.h"

#include "Colorizer.h"
#include "ThermodynamicsSettings.h"

AThermodynamicActor::AThermodynamicActor() {
	PrimaryActorTick.bCanEverTick = true;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(_staticMesh);

	_thermodynamicC = CreateDefaultSubobject<UThermodynamicComponent>(TEXT("Thermodynamic Component"));
	_thermodynamicC->SetupAttachment(RootComponent);
}

void AThermodynamicActor::BeginPlay() {
	Super::BeginPlay();

	const UThermodynamicsSettings* const thermodynamicsSettings = GetDefault<UThermodynamicsSettings>();
	_materialInstance = _staticMesh->CreateDynamicMaterialInstance(0, thermodynamicsSettings->GetThermodynamicsMaterial(), TEXT("Thermodynamics Material"));

	if (_materialInstance != nullptr) {
		_updateMaterialBasedOnTemperature(_thermodynamicC->GetTemperature());
	}

	_thermodynamicC->OnTemperatureChanged.AddUObject(this, &AThermodynamicActor::_updateMaterialBasedOnTemperature);
}

void AThermodynamicActor::_updateMaterialBasedOnTemperature(const double temperature) {
	if (_materialInstance == nullptr) {
		return;
	}

	_materialInstance->SetVectorParameterValue(TEXT("temperature"), FColorizer::GenerateColorFromTemperature(temperature));
}