// Fill out your copyright notice in the Description page of Project Settings.

#include "NewIceWallSkill.h"

void UNewIceWallSkill::Execute(const TObjectPtr<AActor> caster) {
	const auto& actorSpawner = _getActorSpawners()[0];

	const FTransform& casterToWorld = caster->GetTransform();
	const TWeakObjectPtr<AActor> spawnActor = GetWorld()->SpawnActor<AActor>(actorSpawner.ActorClass, actorSpawner.Transform * casterToWorld);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		[spawnActor]() {
			if (spawnActor.IsValid()) {
				spawnActor->Destroy();
			}
		},
		_getDuration(),
		false
	);

	_startCooldown();
}

#if DO_CHECK
void UNewIceWallSkill::CheckParametersSanity() const {
	const auto& actorSpawners = _getActorSpawners();
	check(actorSpawners.Num() == 1);
}
#endif
