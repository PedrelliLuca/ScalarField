// Fill out your copyright notice in the Description page of Project Settings.

#include "ConeOfColdSkill.h"

void UConeOfColdSkill::ExecuteCast(const TObjectPtr<AActor> caster) {
	const auto& coldConeSpawner = _getFollowerActorSpawners()[0];

	_spawnSpringArm = NewObject<USpringArmComponent>(caster, TEXT("Globe SpringArm"));
	_spawnSpringArm->SetupAttachment(caster->GetRootComponent());
	_spawnSpringArm->SetRelativeLocation(FVector::ZeroVector);

	// The point where we have to spawn the globe relative to the caster, it's also the point where the 2nd end of the arm lies
	const FVector coneLocation = coldConeSpawner.Transform.GetLocation();

	// The spring sits on the vector that goes from the caster's root to the globeLocation.
	// About the minus sign:
	// In the spring's reference frame, the spring elongates in the -x direction. Therefore, we need to make the spring point to
	// the location that is the opposite of the want we want the second end to be in.
	_spawnSpringArm->SetRelativeRotation((- coneLocation).Rotation());
	_spawnSpringArm->TargetArmLength = coneLocation.Length();

	_spawnSpringArm->RegisterComponent();

	_spawnActor = GetWorld()->SpawnActor<AActor>(coldConeSpawner.ActorClass, coldConeSpawner.Transform * caster->GetTransform());
	_spawnActor->AttachToComponent(_spawnSpringArm.Get(), FAttachmentTransformRules::KeepWorldTransform, _spawnSpringArm->SocketName);

	check(FMath::IsNearlyEqual(coldConeSpawner.SpawnDuration, GetChannelingTime()));

	_cone = _spawnActor->FindComponentByClass<UCapsuleComponent>();

	_startCooldown();
}

void UConeOfColdSkill::ExecuteChannelingTick(float deltaTime, const TObjectPtr<AActor> caster) {
	FlushPersistentDebugLines(GetWorld());

	if (_cone.IsValid()) {
		DrawDebugCapsule(GetWorld(), _cone->GetComponentLocation(), _cone->GetUnscaledCapsuleHalfHeight(), _cone->GetUnscaledCapsuleRadius(),
			_cone->GetComponentRotation().Quaternion(), FColor::Cyan, false);
	}
}

void UConeOfColdSkill::Abort() {
	Super::Abort();

	_spawnActor->Destroy();
	_spawnSpringArm->DestroyComponent();
	_cone = nullptr;
}

#if DO_CHECK
void UConeOfColdSkill::CheckParametersSanity() const {
	check(_getFollowerActorSpawners().Num() == 1);
}
#endif
