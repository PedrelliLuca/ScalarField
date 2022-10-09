// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SpawnerSkill.h"

void USpawnerSkill::Cast() {
	UE_LOG(LogTemp, Warning, TEXT("USpawnerSkill::Cast()"));
	const FTransform& casterToWorld = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTransform();

	TWeakObjectPtr<AActor> spawnActor = GetWorld()->SpawnActor<AActor>(_spawnClass, _spawnTransform * casterToWorld);

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
}
