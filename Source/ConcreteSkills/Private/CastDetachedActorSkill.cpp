// Fill out your copyright notice in the Description page of Project Settings.

#include "CastDetachedActorSkill.h"

void UCastDetachedActorSkill::_skillCast() {
    auto const& caster = _getCaster();
    check(caster.IsValid());

    FTransform const& casterToWorld = caster->GetTransform();
    TWeakObjectPtr<AActor> const spawnedActor = GetWorld()->SpawnActor<AActor>(_actorClass, _actorToCaster * casterToWorld);

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
        [spawnedActor]() {
            // Something else might have already destroyed the actor.
            if (spawnedActor.IsValid()) {
                spawnedActor->Destroy();
            }
        },
        actualLifetimeSeconds, false);
}

void UCastDetachedActorSkill::_skillChannelingTick(float const deltaTime) {
    // This skill doesn't do anything on channeling
}

void UCastDetachedActorSkill::_skillAbort() {
    // This skill doesn't do anything on abort, the spawned actor is destroyed after _actorLifetimeSeconds
}
