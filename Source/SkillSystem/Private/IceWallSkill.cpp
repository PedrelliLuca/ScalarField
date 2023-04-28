// Fill out your copyright notice in the Description page of Project Settings.

#include "IceWallSkill.h"

void UIceWallSkill::ExecuteCast() {
    const auto& caster = _getCaster();
    check(caster.IsValid());

    const FTransform& casterToWorld = caster->GetTransform();
    const auto& iceWallSpawner = _getActorSpawners()[0];
    const TWeakObjectPtr<AActor> spawnActor = GetWorld()->SpawnActor<AActor>(iceWallSpawner.ActorClass, iceWallSpawner.Transform * casterToWorld);

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        timerHandle,
        [spawnActor]() {
            if (spawnActor.IsValid()) {
                spawnActor->Destroy();
            }
        },
        iceWallSpawner.SpawnDuration, false);

    _startCooldown();
}

#if DO_CHECK
void UIceWallSkill::CheckParametersSanity() const {
    check(_getActorSpawners().Num() == 1);
}
#endif
