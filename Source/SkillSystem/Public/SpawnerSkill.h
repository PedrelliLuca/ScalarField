// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractSkill.h"
#include "SpawnerSkill.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API USpawnerSkill : public UAbstractSkill
{
	GENERATED_BODY()

public:
	bool CastSkill(TObjectPtr<APawn> caster) override;

	const FTransform& GetSpawnTransform() const { return _spawnTransform; }
	const TSubclassOf<AActor>& GetSpawnClass() const { return _spawnClass; }

private:
	// TODO: in the future, the spawn location might work with a targeting system

	// Transform of the object to spawn in the caster's reference frame
	UPROPERTY(EditAnywhere, Category = "Skill Properties")
	FTransform _spawnTransform = FTransform::Identity;
	UPROPERTY(EditAnywhere, Category = "Skill Properties")
	TSubclassOf<AActor> _spawnClass = nullptr;
};
