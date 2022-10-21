// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NewAbstractSkill.h"

#include "SkillsContainerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLSYSTEM_API USkillsContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillsContainerComponent();

	bool ExecuteSkillAtIndex(uint32 index);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Skills")
	TArray<TSubclassOf<UNewAbstractSkill>> _skillsClasses;

	UPROPERTY()
	TArray<TObjectPtr<UNewAbstractSkill>> _skills;
};