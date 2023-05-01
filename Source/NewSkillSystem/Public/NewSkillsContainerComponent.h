// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "NewAbstractSkill.h"

#include "NewSkillsContainerComponent.generated.h"

UCLASS()
class NEWSKILLSYSTEM_API UNewSkillsContainerComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UNewSkillsContainerComponent();

protected:
    void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Skills")
    TArray<TObjectPtr<UNewAbstractSkill>> _skills;
};
