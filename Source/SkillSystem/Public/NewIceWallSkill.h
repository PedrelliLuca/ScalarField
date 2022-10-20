// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NewAbstractSkill.h"

#include "NewIceWallSkill.generated.h"

struct FActorSpawnerSkillComponent {
	TSubclassOf<AActor> ActorClass = nullptr;
	FTransform Transform = FTransform::Identity;
};

/**
 * 
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UNewIceWallSkill : public UNewAbstractSkill {
	GENERATED_BODY()
	
public:
	bool CastSkill(TObjectPtr<APawn> caster) override;
	void InitializeSkill(TWeakObjectPtr<UNewSkillParameters> skillParameters) override;

private:
	FActorSpawnerSkillComponent _actorSpawnerC;
};
