// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parameters/SkillParameters.h"

#include "AbstractSkill.generated.h"


/**
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class SKILLSYSTEM_API UAbstractSkill : public UObject {
	GENERATED_BODY()

public:
	/** TODO: add description */
	virtual void ExecuteCast(TObjectPtr<AActor> caster) PURE_VIRTUAL(UAbstractSkill::Execute, return;);

	virtual void ExecuteChannelingTick(float deltaTime, const TObjectPtr<AActor> caster) {}

	virtual void AbortChanneling() {}

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
	void RemoveAllTargets();

	EMovementCommandMode GetChannelingMovementMode() const { return _parameters.ChannelingMovementMode; }
	bool RequiresChanneling() const { return _parameters.RequiresChanneling; }
	double GetChannelingManaCost() const { return _parameters.ChannelingManaCost; }
	double GetChannelingTime() const { return _parameters.ChannelingTime; }

protected:
	double _getDuration() const { return _parameters.Duration; }
	const TArray<FActorSpawnerParameters>& _getActorSpawners() const { return _parameters.ActorSpawnerParameters; }
	const TArray<FFollowerActorSpawnerParameters>& _getFollowerActorSpawners() const { return _parameters.FollowerActorSpawnerParameters; }
	const TArray<FActorTargetParameters>& _getActorTargets() const { return _parameters.ActorTargetParameters; }

	void _startCooldown();
	void _endCooldown();

private:
	bool _bIsOnCooldown = false;

	UPROPERTY(EditAnywhere)
	FSkillParameters _parameters;
};
