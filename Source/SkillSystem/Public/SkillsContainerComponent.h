// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractSkill.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SkillsContainerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLSYSTEM_API USkillsContainerComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillsContainerComponent();

	TObjectPtr<UAbstractSkill> GetSkillAtIndex(uint32 index) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Skills")
	TArray<TSubclassOf<UAbstractSkill>> _skillsClasses;

	UPROPERTY()
	TArray<TObjectPtr<UAbstractSkill>> _skills;
};