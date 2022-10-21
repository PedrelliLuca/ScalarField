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
	/** TODO: add description */
	virtual void Initialize(TWeakObjectPtr<UNewSkillParameters> skillParameters);

	double GetManaCost() const { return _manaCost; }
	double GetDuration() const { return _duration; }
	bool IsOnCooldown() const { return _bIsOnCooldown; }

protected:
	void _startCooldown();
	void _endCooldown();
	bool _isOnCooldown() { return _bIsOnCooldown; }

private:
	double _manaCost = 0.;
	double _cooldown = 0.;
	double _castTime = 0.;
	double _channelingTime = 0.;
	double _duration = 0.;

	bool _bIsOnCooldown = false;
	
};
