// Fill out your copyright notice in the Description page of Project Settings.

#include "CastDetachedActorSkill.h"

void UCastDetachedActorSkill::_skillCast() {
    const auto& caster = _getCaster();
    check(caster.IsValid());

    const FTransform& casterToWorld = caster->GetTransform();
    const TWeakObjectPtr<AActor> spawnedActor = GetWorld()->SpawnActor<AActor>(_actorClass, _actorToCaster * casterToWorld);

    if (!(_actorLifetimeSeconds < 0.0f)) {
        const auto actualLifetimeSeconds = FMath::IsNearlyZero(_actorLifetimeSeconds) ? _getChannelingSeconds() : _actorLifetimeSeconds;
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
}

void UCastDetachedActorSkill::_skillChannelingTick(const float deltaTime) {
    // This skill doesn't do anything on channeling
}

void UCastDetachedActorSkill::_skillAbort() {
    // This skill doesn't do anything on abort, the spawned actor is destroyed after _actorLifetimeSeconds
}
