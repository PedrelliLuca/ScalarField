// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parameters/NewSkillParameters.h"

#include "NewAbstractSkill.generated.h"


/**
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class SKILLSYSTEM_API UNewAbstractSkill : public UObject {
	GENERATED_BODY()

public:
	/** TODO: add description */
	virtual void Execute(TObjectPtr<AActor> caster) PURE_VIRTUAL(UNewAbstractSkill::Execute, return;);

#if DO_CHECK
	virtual void CheckParametersSanity() const PURE_VIRTUAL(UNewAbstractSkill::CheckParametersSanity, return;);
#endif

	double GetManaCost() const { return _parameters.ManaCost; }
	bool IsOnCooldown() const { return _bIsOnCooldown; }

protected:
	double _getDuration() const { return _parameters.Duration; }
	const TArray<FActorSpawnerParameters>& _getActorSpawners() const { return _parameters.ActorSpawnerParameters; }
	const TArray<FFollowerActorSpawnerParameters>& _getFollowerActorSpawners() const { return _parameters.FollowerActorSpawnerParameters; }

	void _startCooldown();
	void _endCooldown();

private:
	bool _bIsOnCooldown = false;

	UPROPERTY(EditAnywhere)
	FNewSkillParameters _parameters;
};
