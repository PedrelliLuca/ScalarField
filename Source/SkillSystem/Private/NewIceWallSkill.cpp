// Fill out your copyright notice in the Description page of Project Settings.

#include "NewIceWallSkill.h"

bool UNewIceWallSkill::CastSkill(const TObjectPtr<APawn> caster) {
	if (_isOnCooldown()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
		return false;
	}

	const FTransform& casterToWorld = caster->GetTransform();
	const TWeakObjectPtr<AActor> spawnActor = GetWorld()->SpawnActor<AActor>(_actorSpawnerC.ActorClass, _actorSpawnerC.Transform * casterToWorld);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		[spawnActor]() {
			if (spawnActor.IsValid()) {
				spawnActor->Destroy();
			}
		},
		GetDuration(),
		false
	);

	_startCooldown();
	return true;
}

void UNewIceWallSkill::InitializeSkill(const TWeakObjectPtr<UNewSkillParameters> skillParameters) {
	Super::InitializeSkill(skillParameters);

	const auto skillParams = skillParameters.Get();
	check(skillParams->Skill == EConcreteSkill::CS_IceWall);
	check(skillParams->ActorSpawnerParameters.Num() == 1);

	const auto actorSpawnerParams = skillParameters->ActorSpawnerParameters[0];
	_actorSpawnerC.ActorClass = actorSpawnerParams.ActorClass;
	_actorSpawnerC.Transform = actorSpawnerParams.Transform;
}