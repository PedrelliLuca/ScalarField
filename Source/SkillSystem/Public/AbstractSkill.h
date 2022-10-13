// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "AbstractSkill.generated.h"

USTRUCT(BlueprintType)
struct SKILLSYSTEM_API FSkillParameters {
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double ManaCost = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double CoolDown = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double CastTime = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double Duration = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double BaseDamage = 0.;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	TSubclassOf<UAbstractSkill> Class;

	// TODO: add icon property
};

/**
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class SKILLSYSTEM_API UAbstractSkill : public UObject {
	GENERATED_BODY()

public:
	virtual bool CastSkill(TObjectPtr<APawn> caster) PURE_VIRTUAL(UAbstractSkill::Cast, return false;);
	const FSkillParameters& GetParameters() const { return _parameters; }
	void SetParameters(const FSkillParameters& parameters) { _parameters = parameters; }
	double GetManaCost() const { return _parameters.ManaCost; }
	bool IsOnCooldown() const { return _bIsOnCooldown; }
	void StartCooldown();

private:
	void _endCooldown() { _bIsOnCooldown = false; }

	FSkillParameters _parameters{};
	bool _bIsOnCooldown = false;
};
