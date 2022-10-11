// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerSkill.h"

bool USpawnerSkill::CastSkill(const TObjectPtr<APawn> caster) {
	if (IsOnCooldown()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
		return false;
	}

	const FTransform& casterToWorld = caster->GetTransform();
	const TWeakObjectPtr<AActor> spawnActor = GetWorld()->SpawnActor<AActor>(_spawnClass, _spawnTransform * casterToWorld);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		[spawnActor]() {
			if (spawnActor.IsValid()) {
				spawnActor->Destroy();
			}
		},
		GetParameters().Duration,
		false
	);

	StartCooldown();
	return true;
}
