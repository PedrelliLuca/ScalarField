// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnProjectileSkill.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "SkillSpawnedEntityInterface.h"

void USpawnProjectileSkill::_skillCast() {
    auto const& caster = _getCaster();
    check(caster.IsValid());

    // TODO: if 0 targets, the target is simply spawned at _spawnDistance from the spawner in the forward vector direction

    auto const& targets = _getTargets();
    if (!ensureMsgf(!targets.IsEmpty(), TEXT("Must have at least 1 target"))) {
        return;
    }

    for (auto const& target : targets) {
        auto const casterLocation = caster->GetActorLocation();
        auto const targetLocation = target->GetTargetLocation();
        auto const casterToTargetDirection = (targetLocation - casterLocation).GetSafeNormal();
        auto const projectileSpawnLocation = casterLocation + casterToTargetDirection * _spawnDistance;

        auto const projectileTransform = FTransform{projectileSpawnLocation};

        auto const spawnedActor = GetWorld()->SpawnActorDeferred<AActor>(_projectileClass, projectileTransform);
        TWeakInterfacePtr<ISkillSpawnedEntity> const skillSpawnedEntity = Cast<ISkillSpawnedEntity>(spawnedActor);
        if (skillSpawnedEntity.IsValid()) {
            skillSpawnedEntity->SetCaster(_getCaster());
        }

        auto const projectileMovementC = spawnedActor->FindComponentByClass<UProjectileMovementComponent>();
        if (!ensureMsgf(IsValid(projectileMovementC), TEXT("Selected Projectile Class doesn't have a UProjectileMovementComponent"))) {
            return;
        }
        projectileMovementC->Velocity = projectileMovementC->InitialSpeed * casterToTargetDirection;

        spawnedActor->FinishSpawning(projectileTransform);
    }
}

void USpawnProjectileSkill::_skillChannelingTick(float const deltaTime) {
    // This skill doesn't do anything on channeling
}

void USpawnProjectileSkill::_skillAbort() {
    // This skill doesn't do anything on abort, the spawned actor should manage its own destruction
}
