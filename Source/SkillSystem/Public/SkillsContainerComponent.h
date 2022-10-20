// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Parameters/NewSkillParameters.h"

#include "SkillsContainerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLSYSTEM_API USkillsContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillsContainerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Skills")
	TArray<TSubclassOf<UNewSkillParameters>> _skillsParametersClasses;
		
	// TArray<TObjectPtr<UAbstractSkill>> _skills;
};