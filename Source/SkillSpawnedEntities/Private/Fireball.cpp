// Fill out your copyright notice in the Description page of Project Settings.

#include "Fireball.h"

AFireball::AFireball() {
    PrimaryActorTick.bCanEverTick = true;

    _particleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireballParticles"));
    _particleSystem->SetupAttachment(GetRootComponent());
    _fireballTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("FireballTemplate"));

    _projectileMovementC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void AFireball::Tick(const float deltaTime) {
    Super::Tick(deltaTime);

    if (!_caster.IsValid()) {
        Destroy();
        return;
    }

    if (_getThermodynamicComponent()->GetTemperature() < _temperatureExtinctionThreshold) {
        Destroy();
        return;
    }

    const auto distanceFromCaster = (GetActorLocation() - _caster->GetActorLocation()).Length();
    if (distanceFromCaster > _distanceExtinctionThreshold) {
        Destroy();
    }
}

void AFireball::BeginPlay() {
    Super::BeginPlay();

    if (IsValid(_fireballTemplate)) {
        _particleSystem->SetTemplate(_fireballTemplate);
        _particleSystem->Activate(true);
    }
}
