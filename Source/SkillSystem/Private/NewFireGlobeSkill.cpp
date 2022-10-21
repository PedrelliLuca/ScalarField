// Fill out your copyright notice in the Description page of Project Settings.

#include "NewFireGlobeSkill.h"

#include "GameFramework/SpringArmComponent.h"

void UNewFireGlobeSkill::Execute(TObjectPtr<AActor> caster) {
	const auto& fireGlobeSpawner = _getFollowerActorSpawners()[0];

	const TObjectPtr<USpringArmComponent> spawnSpringArm = NewObject<USpringArmComponent>(caster, TEXT("Globe SpringArm"));
	spawnSpringArm->SetupAttachment(caster->GetRootComponent());
	spawnSpringArm->SetRelativeLocation(FVector::ZeroVector);

	// The point where we have to spawn the globe relative to the caster, it's also the point where the 2nd end of the arm lies
	const FVector globeLocation = fireGlobeSpawner.Transform.GetLocation();

	// The spring sits on the vector that goes from the caster's root to the globeLocation
	spawnSpringArm->SetRelativeRotation(globeLocation.Rotation());
	spawnSpringArm->TargetArmLength = globeLocation.Length();

	spawnSpringArm->RegisterComponent();

	const TWeakObjectPtr<AActor> spawnActor = GetWorld()->SpawnActor<AActor>(fireGlobeSpawner.ActorClass, fireGlobeSpawner.Transform * caster->GetTransform());
	spawnActor->AttachToComponent(spawnSpringArm, FAttachmentTransformRules::KeepWorldTransform, spawnSpringArm->SocketName);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		[spawnSpringArm = TWeakObjectPtr<USpringArmComponent>(spawnSpringArm), spawnActor]() {
			if (spawnActor.IsValid()) {
				spawnActor->Destroy();
			}

			if (spawnSpringArm.IsValid()) {
				spawnSpringArm->DestroyComponent();
			}
		},
		_getDuration(),
		false
	);

	_startCooldown();
}

#if DO_CHECK
void UNewFireGlobeSkill::CheckParametersSanity() const {
	check(_getFollowerActorSpawners().Num() == 1);
}
#endif
