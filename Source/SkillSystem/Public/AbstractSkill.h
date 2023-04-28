// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCastCondition.h"
#include "Parameters/SkillParameters.h"

#include "AbstractSkill.generated.h"

/**
 *
 */
UCLASS(NotBlueprintable, Abstract)
class SKILLSYSTEM_API UAbstractSkill : public UObject {
    GENERATED_BODY()

public:
    /** \brief Sets the caster of the skill. This function must be called immediately after skill creation: the caster is a crucial for skills' execution.
     * The caster should be a parameter for the skill's constructor, but since Unreal does not allow constructor arguments with NewObject I was forced to make
     * this function.
     * The skill's caster can only be set once, subsequent calls won't do anything.
     */
    void SetCaster(TObjectPtr<AActor> caster);

    /** \brief Casts the skill. The function check()s for the skill's caster, so make sure you set it before calling this. */
    virtual void ExecuteCast() PURE_VIRTUAL(UAbstractSkill::Execute, return;);
    /** \brief Executes a tick of the skill's channeling process. The function check()s for the skill's caster, so make sure you set it before calling this. */
    virtual void ExecuteChannelingTick(float deltaTime, const TObjectPtr<AActor> caster) {}

    virtual void Abort() { _removeAllTargets(); }

#if DO_CHECK
    virtual void CheckParametersSanity() const {}
#endif

    bool IsOnCooldown() const { return _bIsOnCooldown; }

    EMovementCommandMode GetCastMovementMode() const { return _parameters.CastMovementMode; }
    double GetCastManaCost() const { return _parameters.CastManaCost; }
    double GetCastTime() const { return _parameters.CastTime; }

    EMovementCommandMode GetTargetingMovementMode() const { return _parameters.TargetingMovementMode; }
    uint32 NumberOfTargets() const { return _parameters.ActorTargetParameters.Num(); }
    bool RequiresTarget() const { return NumberOfTargets() > 0; }
    virtual bool IsValidTarget(int32 targetIndex, TObjectPtr<AActor> target) const;
    double GetMaxDistanceForTarget(int32 targetIndex) const;
    void SetTarget(int32 targetIndex, TWeakObjectPtr<AActor> target);

    EMovementCommandMode GetChannelingMovementMode() const { return _parameters.ChannelingMovementMode; }
    bool RequiresChanneling() const { return _parameters.RequiresChanneling; }
    double GetChannelingManaCost() const { return _parameters.ChannelingManaCost; }
    double GetChannelingTime() const { return _parameters.ChannelingTime; }

protected:
    const TArray<FActorSpawnerParameters>& _getActorSpawners() const { return _parameters.ActorSpawnerParameters; }
    const TArray<FFollowerActorSpawnerParameters>& _getFollowerActorSpawners() const { return _parameters.FollowerActorSpawnerParameters; }
    const TArray<FActorTargetParameters>& _getActorTargets() const { return _parameters.ActorTargetParameters; }
    void _removeAllTargets();

    void _startCooldown();
    void _endCooldown();

    const TWeakObjectPtr<AActor>& _getCaster() const { return _caster; }

    // Allows for a set of initial items to be added to our instances.
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Conditions")
    TArray<USkillCastCondition*> _castConditions;

private:
    bool _bIsOnCooldown = false;

    UPROPERTY(EditAnywhere)
    FSkillParameters _parameters;

    TWeakObjectPtr<AActor> _caster;
};
