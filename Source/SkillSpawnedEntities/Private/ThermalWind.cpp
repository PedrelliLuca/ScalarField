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

	FlushPersistentDebugLines(GetWorld());
	
	_timeSinceSpawn = FMath::Clamp(_timeSinceSpawn + deltaTime, 0., _lifetime);
	const double alpha = _timeSinceSpawn / _lifetime;
	const double halfHeight = FMath::Lerp(_minCapsuleHalfHeight, _maxCapsuleHalfHeight, alpha);
	const double radius = FMath::Lerp(_minCapsuleRadius, _maxCapsuleRadius, alpha);
	
	const auto impactCapsule = _impactOneHitDmgDealerC->GetImpactCapsule();
	
	impactCapsule->SetCapsuleHalfHeight(halfHeight);
	impactCapsule->SetCapsuleRadius(radius);
	impactCapsule->SetRelativeLocation(-FVector::ForwardVector * halfHeight);
	
	DrawDebugCapsule(GetWorld(), impactCapsule->GetComponentLocation(), impactCapsule->GetUnscaledCapsuleHalfHeight(), impactCapsule->GetUnscaledCapsuleRadius(),
		impactCapsule->GetComponentRotation().Quaternion(), FColor::Green, false);
}

void AThermalWind::BeginPlay() {
	Super::BeginPlay();

	if (_lifetime == 0.0) {
		UE_LOG(LogTemp, Warning, TEXT("%s(): Invalid lifetime. Use UWorld::SpawnActorDeferred() to spawn this actor and call SetLifetime()."), *FString{__FUNCTION__});
		return;
	}

	const auto impactCapsule = _impactOneHitDmgDealerC->GetImpactCapsule();

	impactCapsule->SetRelativeLocation(-FVector::ForwardVector * _minCapsuleHalfHeight);
	impactCapsule->SetCapsuleHalfHeight(_minCapsuleHalfHeight);
	impactCapsule->SetCapsuleRadius(_minCapsuleRadius);

	if(!_caster.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("%s(): Invalid caster. Use UWorld::SpawnActorDeferred() to spawn this actor and call SetCaster()."), *FString{__FUNCTION__});
		return;
	}

	_impactOneHitDmgDealerC->AddImmuneActor(_caster);
	
	// Particle system's setup
	TObjectPtr<UParticleSystem> activeParticleTemplate = nullptr;
	if (const auto thermoC = Cast<UThermodynamicComponent>(_caster->GetComponentByClass(UThermodynamicComponent::StaticClass()))) {
		if (thermoC->GetTemperature() > _hotThreshold) {
			UE_LOG(LogTemp, Warning, TEXT("BURN!!!"));
			activeParticleTemplate = _hotTemplate;
		}
		else if (thermoC->GetTemperature() < _coldThreshold) {
			UE_LOG(LogTemp, Warning, TEXT("FREEZE!!!"));
			activeParticleTemplate = _coldTemplate;
		}
	}
	
	if (IsValid(activeParticleTemplate)) {
		_particleSystem->SetTemplate(activeParticleTemplate);
		_particleSystem->Activate(true);
	}
	
	_timeSinceSpawn = 0.0;
}
