// Fill out your copyright notice in the Description page of Project Settings.


#include "NewThermodynamicActor.h"

#include "Colorizer.h"
#include "ThermodynamicsSettings.h"

ANewThermodynamicActor::ANewThermodynamicActor() {
	PrimaryActorTick.bCanEverTick = true;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(_staticMesh);

	_simpleThermodynamicCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Thermodynamic Capsule"));
	_simpleThermodynamicCollision->SetupAttachment(RootComponent);
	_simpleThermodynamicCollision->SetCollisionProfileName("HeatExchanger");

	_thermodynamicC = CreateDefaultSubobject<UNewThermodynamicComponent>(TEXT("Thermodynamic Component"));
}

void ANewThermodynamicActor::SetThermicCapsuleDimensions(const double radius, const double halfHeight) {
	_simpleThermodynamicCollision->SetCapsuleRadius(radius);
	_simpleThermodynamicCollision->SetCapsuleHalfHeight(halfHeight);
}

void ANewThermodynamicActor::BeginPlay() {
	Super::BeginPlay();

	const auto complexThermalCollisions = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName{ "ComplexThermalCollision" });
	check(complexThermalCollisions.Num() <= 1);

	if (!complexThermalCollisions.IsEmpty()) {
		_complexThermoCollision = Cast<UPrimitiveComponent>(complexThermalCollisions[0]);
	}
	_thermodynamicC->SetCollision(_simpleThermodynamicCollision, _complexThermoCollision);

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