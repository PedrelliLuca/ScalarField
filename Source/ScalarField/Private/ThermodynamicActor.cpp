// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicActor.h"

AThermodynamicActor::AThermodynamicActor() {
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(_mesh);

	_thermodynamicC = CreateDefaultSubobject<UThermodynamicComponent>(TEXT("Thermodynamic Component"));
	_thermodynamicC->SetupAttachment(RootComponent);
}
