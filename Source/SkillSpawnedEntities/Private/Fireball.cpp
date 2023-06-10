// Fill out your copyright notice in the Description page of Project Settings.

#include "Fireball.h"

AFireball::AFireball() {
    PrimaryActorTick.bCanEverTick = true;

    _staticMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(_staticMeshC);

    _particleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireballParticles"));
    _particleSystem->SetupAttachment(GetRootComponent());
    _fireTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("FireTemplate"));

    _thermodynamicC = CreateDefaultSubobject<UThermodynamicComponent>(TEXT("Thermodynamics"));
}

void AFireball::Tick(const float deltaTime) {
    Super::Tick(deltaTime);
}

void AFireball::BeginPlay() {
    Super::BeginPlay();
}
