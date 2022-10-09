// Fill out your copyright notice in the Description page of Project Settings.

#include "FireGlobeSkill.h"

bool UFireGlobeSkill::Cast(const TObjectPtr<APawn> caster) {
	if (IsOnCooldown()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
		return false;
	}

	// TODO: do not retrieve this, have it has Cast() argument
	const TObjectPtr<USpringArmComponent> spawnSpringArm = NewObject<USpringArmComponent>(caster, TEXT("Globe SpringArm"));
	spawnSpringArm->SetupAttachment(caster->GetRootComponent());

	spawnSpringArm->SetRelativeLocation(FVector::ZeroVector);
	// The point where we have to spawn the globe relative to the caster, it's also the point where the 2nd end of the arm lies
	const FVector endLocation = GetSpawnTransform().GetLocation();
	spawnSpringArm->SetRelativeRotation(endLocation.Rotation());
	spawnSpringArm->TargetArmLength = endLocation.Length();

	spawnSpringArm->RegisterComponent();

	const TWeakObjectPtr<AActor> spawnActor = GetWorld()->SpawnActor<AActor>(GetSpawnClass(), GetSpawnTransform() * caster->GetTransform());
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
		GetParameters().Duration,
		false
	);

	StartCooldown();
	return true;
}
