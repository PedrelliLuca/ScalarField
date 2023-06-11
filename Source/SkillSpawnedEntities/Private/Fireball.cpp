// Fill out your copyright notice in the Description page of Project Settings.

#include "Fireball.h"

AFireball::AFireball() {
    PrimaryActorTick.bCanEverTick = true;

    _staticMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(_staticMeshC);

    _particleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireballParticles"));
    _particleSystem->SetupAttachment(GetRootComponent());
    _fireballTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("FireballTemplate"));

    _thermodynamicC = CreateDefaultSubobject<UThermodynamicComponent>(TEXT("Thermodynamics"));

    _projectileMovementC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void AFireball::Tick(const float deltaTime) {
    Super::Tick(deltaTime);

    if (!_caster.IsValid()) {
        Destroy();
        return;
    }

    if (_thermodynamicC->GetTemperature() < _temperatureExtinctionThreshold) {
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

    const auto simpleThermalCollisions = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName{"SimpleThermalCollision"});
    check(simpleThermalCollisions.Num() == 1);
    _thermodynamicC->SetCollision(Cast<UPrimitiveComponent>(simpleThermalCollisions.Last()), nullptr);

    // Setting up the DMI that changes the mesh color based on temperature
    // const UThermodynamicsSettings* const thermodynamicsSettings = GetDefault<UThermodynamicsSettings>();
    // _materialInstance = _staticMesh->CreateDynamicMaterialInstance(0, thermodynamicsSettings->GetThermodynamicsMaterial(), TEXT("Thermodynamics Material"));
    //
    // if (_materialInstance != nullptr) {
    //     _updateMaterialBasedOnTemperature(_thermodynamicC->GetTemperature());
    //     _thermodynamicC->OnTemperatureChanged.AddUObject(this, &AThermodynamicActor::_updateMaterialBasedOnTemperature);
    // }
}
