// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NewAbstractSkill.h"

#include "SpawnProjectileSkill.generated.h"

/**
 * \brief A cast-only skill that spawns an actor in the world. Once casted, the caster doesn't have control over the actor's lifetime.
 */
UCLASS(Blueprintable)
class CONCRETESKILLS_API USpawnProjectileSkill : public UNewAbstractSkill {
    GENERATED_BODY()

protected:
    // TODO: this can be generalized in the future by having an array of FSkillSpawnParameters struct collecting these 3 values.

    /** \brief Class of the actor to spawn. Must implement the ISkillSpawnedEntity interface. */
    UPROPERTY(EditAnywhere, Category = "Projectile", meta = (MustImplement = "/Script/SkillSpawnedEntities.SkillSpawnedEntity"))
    TSubclassOf<AActor> _projectileClass = nullptr;

    /** \brief How far from the caster the projectile will spawn */
    UPROPERTY(EditAnywhere, Category = "Projectile")
    float _spawnDistance = 100.0f;

private:
    void _skillCast() override;
    void _skillChannelingTick(float deltaTime) override;
    void _skillAbort() override;
};
