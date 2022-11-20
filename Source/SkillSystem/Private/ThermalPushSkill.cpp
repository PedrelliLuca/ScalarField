// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermalPushSkill.h"

#include "Particles/ParticleSystemComponent.h"
#include "NewThermodynamicComponent.h"

UThermalPushSkill::UThermalPushSkill() {
	_hotTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("Hot Particle System"));
	_coldTemplate = CreateDefaultSubobject<UParticleSystem>(TEXT("Cold Particle System"));
}

void UThermalPushSkill::ExecuteCast(TObjectPtr<AActor> caster) {
	// Capsule component's setup
	_pushCapsule = NewObject<UCapsuleComponent>(caster, TEXT("Thermal Push Capsule"));
	_pushCapsule->SetupAttachment(caster->GetRootComponent());
	_pushCapsule->SetRelativeLocation(FVector::ForwardVector * _minCapsuleHalfHeight);
	_pushCapsule->SetRelativeRotation(FRotator{ 90., 0., 0. });
	_pushCapsule->SetCollisionProfileName("BlockAllDynamic");
	_pushCapsule->SetCapsuleHalfHeight(_minCapsuleHalfHeight);
	_pushCapsule->SetCapsuleRadius(_minCapsuleRadius);
	_pushCapsule->bMultiBodyOverlap = true;

	_pushCapsule->RegisterComponent();

	// Particle system's setup
	TWeakObjectPtr<UParticleSystem> activeParticleTemplate = nullptr;
	if (const auto thermoC = Cast<UNewThermodynamicComponent>(caster->GetComponentByClass(UNewThermodynamicComponent::StaticClass()))) {
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
		_activeParticleSystem = NewObject<UParticleSystemComponent>(caster, TEXT("Push Particle System"));
		_activeParticleSystem->SetupAttachment(_pushCapsule.Get());
		_activeParticleSystem->SetTemplate(activeParticleTemplate.Get());

		_activeParticleSystem->RegisterComponent();
		_activeParticleSystem->Activate(true);
	}

	_timeFromCast = 0.;
	_startCooldown();
}

void UThermalPushSkill::ExecuteChannelingTick(float deltaTime, const TObjectPtr<AActor> caster) {
	FlushPersistentDebugLines(GetWorld());

	const double channelingTime = GetChannelingTime();
	_timeFromCast = FMath::Clamp(_timeFromCast + deltaTime, 0., channelingTime);
	const double alpha = _timeFromCast / channelingTime;
	const double halfHeight = FMath::Lerp(_minCapsuleHalfHeight, _maxCapsuleHalfHeight, alpha);
	const double radius = FMath::Lerp(_minCapsuleRadius, _maxCapsuleRadius, alpha);

	// ExecuteCast() must be called before ExecuteChannelingTick()
	check(_pushCapsule.IsValid());

	_pushCapsule->SetCapsuleHalfHeight(halfHeight);
	_pushCapsule->SetCapsuleRadius(radius);
	_pushCapsule->SetRelativeLocation(FVector::ForwardVector * halfHeight);

	DrawDebugCapsule(GetWorld(), _pushCapsule->GetComponentLocation(), _pushCapsule->GetUnscaledCapsuleHalfHeight(), _pushCapsule->GetUnscaledCapsuleRadius(),
		_pushCapsule->GetComponentRotation().Quaternion(), FColor::Green, false);
}

void UThermalPushSkill::Abort() {
	Super::Abort();

	if (_activeParticleSystem.IsValid()) {
		_activeParticleSystem->DestroyComponent();
	}

	// Are you calling this function on a casted thermal push?
	check(_pushCapsule.IsValid());

	_pushCapsule->DestroyComponent();
}
