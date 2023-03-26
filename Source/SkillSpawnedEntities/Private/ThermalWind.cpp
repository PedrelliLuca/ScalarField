// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermalWind.h"

#include "ThermodynamicComponent.h"

AThermalWind::AThermalWind() {
    PrimaryActorTick.bCanEverTick = true;

    _impactOneHitDmgDealerC = CreateDefaultSubobject<UImpactOneHitDamageDealerComponent>(TEXT("Impact One Hit Dmg Dealer"));
    if (const auto impactCapsule = _impactOneHitDmgDealerC->GetImpactCapsule()) {
        SetRootComponent(impactCapsule);
    }

    _particleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Wind Particles"));
    _particleSystem->SetupAttachment(GetRootComponent());

    _hotTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("Hot Particle System"));
    _coldTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("Cold Particle System"));
}

void AThermalWind::Tick(const float deltaTime) {
    Super::Tick(deltaTime);

    const auto impactCapsule = _impactOneHitDmgDealerC->GetImpactCapsule();
    DrawDebugCapsule(GetWorld(), impactCapsule->GetComponentLocation(), impactCapsule->GetUnscaledCapsuleHalfHeight(),
        impactCapsule->GetUnscaledCapsuleRadius(), impactCapsule->GetComponentRotation().Quaternion(), FColor::Green, false);
}

void AThermalWind::BeginPlay() {
    Super::BeginPlay();

    if (!_caster.IsValid()) {
        UE_LOG(
            LogTemp, Warning, TEXT("%s(): Invalid caster. Use UWorld::SpawnActorDeferred() to spawn this actor and call SetCaster()."), *FString{__FUNCTION__});
        return;
    }

    _impactOneHitDmgDealerC->AddImmuneActor(_caster);
    _impactOneHitDmgDealerC->SetImpulseCenter(_caster->GetActorLocation());

    // Particle system's setup
    TObjectPtr<UParticleSystem> activeParticleTemplate = nullptr;
    if (const auto thermoC = Cast<UThermodynamicComponent>(_caster->GetComponentByClass(UThermodynamicComponent::StaticClass()))) {
        if (thermoC->GetTemperature() > _hotThreshold) {
            UE_LOG(LogTemp, Warning, TEXT("BURN!!!"));
            activeParticleTemplate = _hotTemplate;
        } else if (thermoC->GetTemperature() < _coldThreshold) {
            UE_LOG(LogTemp, Warning, TEXT("FREEZE!!!"));
            activeParticleTemplate = _coldTemplate;
        }
    }

    if (IsValid(activeParticleTemplate)) {
        _particleSystem->SetTemplate(activeParticleTemplate);
        _particleSystem->Activate(true);
    }
}
