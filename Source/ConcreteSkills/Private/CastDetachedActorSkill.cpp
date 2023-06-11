// Fill out your copyright notice in the Description page of Project Settings.

#include "CastDetachedActorSkill.h"

#include "Algo/ForEach.h"

void UCastDetachedActorSkill::_skillCast() {
    const auto& caster = _getCaster();
    check(caster.IsValid());

    const FTransform& casterToWorld = caster->GetTransform();

    TArray<TObjectPtr<AActor>> spawnedActors;

    if (_getTargets().Num() > 0) {
        Algo::Transform(_getTargets(), spawnedActors,
            [this](const auto& target) { return GetWorld()->SpawnActor<AActor>(_actorClass, FTransform{target->GetTargetLocation()}); });
    } else {
        spawnedActors.Emplace(GetWorld()->SpawnActor<AActor>(_actorClass, _actorToCaster * casterToWorld));
    }

    float actualLifetimeSeconds;
    if (FMath::IsNearlyZero(_actorLifetimeSeconds)) {
        actualLifetimeSeconds = _getChannelingSeconds();
    } else if (_actorLifetimeSeconds > 0.0f) {
        actualLifetimeSeconds = _actorLifetimeSeconds;
    } else {
        return;
    }

    // TODO (maybe?): make a core "add-ons" module with an ISelfDestructing interface and an ASelfDestructing actor. Such actors handle destruction on their
    // own by having a SetSelfDestruction(float countdown) function that sets up this timer.
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        timerHandle,
        [spawnedActors]() {
            Algo::ForEach(spawnedActors, [](const auto& spawnedActor) {
                // Something else might have already destroyed the actor.
                if (IsValid(spawnedActor)) {
                    spawnedActor->Destroy();
                }
            });
        },
        actualLifetimeSeconds, false);
}

void UCastDetachedActorSkill::_skillChannelingTick(const float deltaTime) {
    // This skill doesn't do anything on channeling
}

void UCastDetachedActorSkill::_skillAbort() {
    // This skill doesn't do anything on abort, the spawned actor is destroyed after _actorLifetimeSeconds
}
