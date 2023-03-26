// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "GameFramework/SpringArmComponent.h"

#include "ThermalPushSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UThermalPushSkill : public UAbstractSkill {
	GENERATED_BODY()
	
public:
	void ExecuteCast(TObjectPtr<AActor> caster) override;
	void Abort() override;

private:
	void _cleanupCallback();
	
	FTimerHandle _timerHandle;

	TWeakObjectPtr<USpringArmComponent> _spawnSpringArm = nullptr;
	TWeakObjectPtr<AActor> _spawnActor = nullptr;
};
