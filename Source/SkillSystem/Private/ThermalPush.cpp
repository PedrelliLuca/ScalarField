// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermalPush.h"

#include "Particles/ParticleSystemComponent.h"
#include "ThermodynamicComponent.h"

UThermalPush::UThermalPush() {
	_hotTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("Hot Particle System"));
	_coldTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("Cold Particle System"));
}

bool UThermalPush::CastSkill(const TObjectPtr<APawn> caster) {
	if (IsOnCooldown()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
		return false;
	}

	_spawnCapsule = NewObject<UCapsuleComponent>(caster, TEXT("Thermal Push Capsule"));
	_spawnCapsule->SetupAttachment(caster->GetRootComponent());
	_spawnCapsule->SetRelativeLocation(FVector::ForwardVector * _minCapsuleHalfHeight);
	_spawnCapsule->SetRelativeRotation(FRotator{ 90., 0., 0. });
	_spawnCapsule->SetCollisionProfileName("BlockAllDynamic");
	_spawnCapsule->SetCapsuleHalfHeight(_minCapsuleHalfHeight);
	_spawnCapsule->SetCapsuleRadius(_minCapsuleRadius);
	_spawnCapsule->bMultiBodyOverlap = true;

	_spawnCapsule->RegisterComponent();

	TWeakObjectPtr<UParticleSystemComponent> activeParticleSystem = nullptr;
	TWeakObjectPtr<UParticleSystem> activeParticleTemplate = nullptr;

	if (const auto thermoC = Cast<UThermodynamicComponent>(caster->GetComponentByClass(UThermodynamicComponent::StaticClass()))) {
		if (thermoC->GetTemperature() > _hotThreshold) {
			UE_LOG(LogTemp, Warning, TEXT("BURN!!!"));
			activeParticleTemplate = _hotTemplate;
		}
		else if (thermoC->GetTemperature() < _coldThreshold) {
			UE_LOG(LogTemp, Warning, TEXT("FREEZE!!!"));
			activeParticleTemplate = _coldTemplate;
		}
	}

	if (activeParticleTemplate.IsValid()) {
		activeParticleSystem = NewObject<UParticleSystemComponent>(caster, TEXT("Push Particle System"));
		activeParticleSystem->SetupAttachment(_spawnCapsule.Get());
		activeParticleSystem->SetTemplate(activeParticleTemplate.Get());

		activeParticleSystem->RegisterComponent();
		activeParticleSystem->Activate(true);
	}

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		[spawnCapsule = TWeakObjectPtr<UCapsuleComponent>{ _spawnCapsule }, activeParticleSystem]() {
			if (activeParticleSystem.IsValid()) {
				activeParticleSystem->DestroyComponent();
			}

			if (spawnCapsule.IsValid()) {
				spawnCapsule->DestroyComponent();
			}
		},
		GetParameters().Duration,
		false
	);

	_timeFromCast = 0.;
	StartCooldown();
	return true;
}

void UThermalPush::Tick(float deltaTime) {
	FlushPersistentDebugLines(GetWorld());

	_timeFromCast = FMath::Clamp(_timeFromCast + deltaTime, 0., GetParameters().Duration);
	const double alpha = _timeFromCast / GetParameters().Duration;
	const double halfHeight = FMath::Lerp(_minCapsuleHalfHeight, _maxCapsuleHalfHeight, alpha);
	const double radius = FMath::Lerp(_minCapsuleRadius, _maxCapsuleRadius, alpha);

	// This should be ensured by IsAllowedToTick() override
	check(_spawnCapsule.IsValid());

	_spawnCapsule->SetCapsuleHalfHeight(halfHeight);
	_spawnCapsule->SetCapsuleRadius(radius);
	_spawnCapsule->SetRelativeLocation(FVector::ForwardVector * halfHeight);

	DrawDebugCapsule(GetWorld(), _spawnCapsule->GetComponentLocation(), _spawnCapsule->GetUnscaledCapsuleHalfHeight(), _spawnCapsule->GetUnscaledCapsuleRadius(), _spawnCapsule->GetComponentRotation().Quaternion(), FColor::Green, false, GetParameters().Duration);

}

TStatId UThermalPush::GetStatId() const {
	return TStatId();
}
