// Fill out your copyright notice in the Description page of Project Settings.


#include "NewThermodynamicActor.h"

#include "Colorizer.h"
#include "ThermodynamicsSettings.h"

ANewThermodynamicActor::ANewThermodynamicActor() {
	PrimaryActorTick.bCanEverTick = true;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(_staticMesh);

	_thermodynamicC = CreateDefaultSubobject<UNewThermodynamicComponent>(TEXT("Thermodynamic Component"));
}

void ANewThermodynamicActor::BeginPlay() {
	Super::BeginPlay();

	const auto simpleThermalCollisions = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName{ "SimpleThermalCollision" });
	check(simpleThermalCollisions.Num() == 1);
	_simpleThermalCollision = Cast<UPrimitiveComponent>(simpleThermalCollisions[0]);

	const auto complexThermalCollisions = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName{ "ComplexThermalCollision" });
	check(complexThermalCollisions.Num() <= 1);

	if (!complexThermalCollisions.IsEmpty()) {
		_complexThermalCollision = Cast<UPrimitiveComponent>(complexThermalCollisions[0]);
	}
	_thermodynamicC->SetCollision(_simpleThermalCollision, _complexThermalCollision);

	// Setting up the DMI that changes the mesh color based on temperature
	const UThermodynamicsSettings* const thermodynamicsSettings = GetDefault<UThermodynamicsSettings>();
	_materialInstance = _staticMesh->CreateDynamicMaterialInstance(0, thermodynamicsSettings->GetThermodynamicsMaterial(), TEXT("Thermodynamics Material"));

	if (_materialInstance != nullptr) {
		_updateMaterialBasedOnTemperature(_thermodynamicC->GetTemperature());
		_thermodynamicC->OnTemperatureChanged.AddUObject(this, &ANewThermodynamicActor::_updateMaterialBasedOnTemperature);
	}
}

void ANewThermodynamicActor::_updateMaterialBasedOnTemperature(const double temperature) {
	check(!_materialInstance.IsNull())
	_materialInstance->SetVectorParameterValue(TEXT("temperature"), FColorizer::GenerateColorFromTemperature(temperature));
}