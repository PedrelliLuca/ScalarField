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
	virtual bool CastSkill(TObjectPtr<APawn> caster) PURE_VIRTUAL(UNewAbstractSkill::CastSkill, return false;);
	/** TODO: add description */
	virtual void InitializeSkill(TWeakObjectPtr<UNewSkillParameters> skillParameters);

	double GetDuration() const { return _duration; }

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
