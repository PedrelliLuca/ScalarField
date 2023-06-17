// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnProjectileSkill.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "SkillSpawnedEntityInterface.h"

void USpawnProjectileSkill::_skillCast() {
    const auto& caster = _getCaster();
    check(caster.IsValid());

    // TODO: if 0 targets, the target is simply spawned at _spawnDistance from the spawner in the forward vector direction

    const auto& targets = _getTargets();
    if (!ensureMsgf(!targets.IsEmpty(), TEXT("Must have at least 1 target"))) {
        return;
    }

    for (const auto& target : targets) {
        const auto casterLocation = caster->GetActorLocation();
        const auto targetLocation = target->GetTargetLocation();
        const auto casterToTargetDirection = (targetLocation - casterLocation).GetSafeNormal();
        const auto projectileSpawnLocation = casterLocation + casterToTargetDirection * _spawnDistance;

        const auto projectileTransform = FTransform{projectileSpawnLocation};

        const auto spawnedActor = GetWorld()->SpawnActorDeferred<AActor>(_projectileClass, projectileTransform);
        const TWeakInterfacePtr<ISkillSpawnedEntity> skillSpawnedEntity = Cast<ISkillSpawnedEntity>(spawnedActor);
        if (skillSpawnedEntity.IsValid()) {
            skillSpawnedEntity->SetCaster(_getCaster());
        }

        const auto projectileMovementC = spawnedActor->FindComponentByClass<UProjectileMovementComponent>();
        if (!ensureMsgf(IsValid(projectileMovementC), TEXT("Selected Projectile Class doesn't have a UProjectileMovementComponent"))) {
            return;
        }
        projectileMovementC->Velocity = projectileMovementC->InitialSpeed * casterToTargetDirection;
        
        spawnedActor->FinishSpawning(projectileTransform);
    }
}

void USpawnProjectileSkill::_skillChannelingTick(const float deltaTime) {
    // This skill doesn't do anything on channeling
}

void USpawnProjectileSkill::_skillAbort() {
    // This skill doesn't do anything on abort, the spawned actor should manage its own destruction
}
