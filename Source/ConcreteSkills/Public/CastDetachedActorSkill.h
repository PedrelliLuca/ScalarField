// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NewAbstractSkill.h"

#include "CastDetachedActorSkill.generated.h"

/**
 * \brief A cast-only skill that spawns an actor in the world. Once casted, the caster doesn't have control over the actor's lifetime.
 */
UCLASS(Blueprintable)
class CONCRETESKILLS_API UCastDetachedActorSkill : public UNewAbstractSkill {
    GENERATED_BODY()

protected:
    // TODO: this can be generalized in the future by having an array of FSkillSpawnParameters struct collecting these 3 values.

    /** \brief Class of the actor to spawn. Must implement the ISkillSpawnedEntity interface. */
    UPROPERTY(EditAnywhere, Category = "Detached Actor", meta = (MustImplement = "/Script/SkillSpawnedEntities.SkillSpawnedEntity"))
    TSubclassOf<AActor> _actorClass = nullptr;

    /** \brief Transform of the actor to spawn relative to its parent. This actor won't be attached to the caster, so this relative transform only holds when
     * spawning. */
    UPROPERTY(EditAnywhere, Category = "Detached Actor")
    FTransform _actorToCaster = FTransform::Identity;

    /** \brief Lifetime of the actor to spawn. A negative value means infinite lifetime. A value of zero means the lifetime is equal to the channeling seconds.
     */
    UPROPERTY(EditAnywhere, Category = "Detached Actor")
    float _actorLifetimeSeconds = -1.f;

private:
    void _skillCast() override;
    void _skillChannelingTick(float deltaTime) override;
    void _skillAbort() override;
};
