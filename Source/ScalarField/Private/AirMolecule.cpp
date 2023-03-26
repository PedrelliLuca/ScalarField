// Fill out your copyright notice in the Description page of Project Settings.

#include "AirMolecule.h"

AAirMolecule::AAirMolecule() {
    PrimaryActorTick.bCanEverTick = true;

    _capsuleThermalCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SimpleThermalCollision"));
    _capsuleThermalCollision->SetupAttachment(RootComponent);
    _capsuleThermalCollision->SetCollisionProfileName(TEXT("HeatExchanger"));
    _capsuleThermalCollision->ComponentTags.Emplace(TEXT("SimpleThermalCollision"));
}

void AAirMolecule::SetThermalCapsuleDimensions(const double radius, const double halfHeight) {
    _capsuleThermalCollision->SetCapsuleRadius(radius);
    _capsuleThermalCollision->SetCapsuleHalfHeight(halfHeight);
}
