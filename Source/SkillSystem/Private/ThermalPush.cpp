// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermalPush.h"

#include "Components/CapsuleComponent.h"

bool UThermalPush::Cast(const TObjectPtr<APawn> caster) {
	if (IsOnCooldown()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
		return false;
	}

	const TObjectPtr<UCapsuleComponent> spawnCapsule = NewObject<UCapsuleComponent>(caster, TEXT("Thermal Push Capsule"));
	spawnCapsule->SetupAttachment(caster->GetRootComponent());
	spawnCapsule->SetRelativeLocation(FVector::ForwardVector * 50.);
	spawnCapsule->SetRelativeRotation(FRotator{ 90., 0., 0. });
	spawnCapsule->SetCollisionProfileName("BlockAllDynamic");

	spawnCapsule->RegisterComponent();

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		[spawnCapsule = TWeakObjectPtr<UCapsuleComponent>{spawnCapsule}]() {
			if (spawnCapsule.IsValid()) {
				spawnCapsule->DestroyComponent();
			}
		},
		GetParameters().Duration,
		false
	);

	StartCooldown();
	return true;
}
