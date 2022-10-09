// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SpawnerSkill.h"

void USpawnerSkill::Cast() {
	UE_LOG(LogTemp, Warning, TEXT("USpawnerSkill::Cast()"));
	const FTransform& charToWorld = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTransform();

	GetWorld()->SpawnActor<AActor>(_spawnClass, _spawnTransform * charToWorld);
}
