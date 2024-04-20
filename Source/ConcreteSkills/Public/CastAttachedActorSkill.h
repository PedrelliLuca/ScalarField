// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "GameFramework/SpringArmComponent.h"

#include "CastAttachedActorSkill.generated.h"

/**
 * \brief A cast-only skill that spawns an actor attached to the target. The attachment lasts for the entire channeling phase.
 */
UCLASS(Blueprintable)
class CONCRETESKILLS_API UCastAttachedActorSkill : public UAbstractSkill {
    GENERATED_BODY()

protected:
    // TODO: this can be generalized in the future by having an array of FSkillSpawnParameters struct collecting these 3 values.

    /** \brief Class of the actor to spawn. Must implement the ISkillSpawnedEntity interface. */
    UPROPERTY(EditAnywhere, Category = "Detached Actor", meta = (MustImplement = "/Script/SkillSpawnedEntities.SkillSpawnedEntity"))
    TSubclassOf<AActor> _actorClass = nullptr;

    /** \brief Transform of the actor to spawn relative to its parent. This actor won't be attached to the caster, so this relative transform only holds when
     * spawning. */
    UPROPERTY(EditAnywhere, Category = "Detached Actor")
    FTransform _actorToTarget = FTransform::Identity;

private:
    void _skillCast() override;
    void _skillChannelingTick(float deltaTime) override;
    void _skillAbort() override;

    TWeakObjectPtr<USpringArmComponent> _spawnSpringArm = nullptr;
    TWeakObjectPtr<AActor> _spawnActor = nullptr;
};
