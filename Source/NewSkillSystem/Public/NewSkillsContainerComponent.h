// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "NewAbstractSkill.h"
#include "SkillCastResult.h"

#include "NewSkillsContainerComponent.generated.h"

UCLASS()
class NEWSKILLSYSTEM_API UNewSkillsContainerComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UNewSkillsContainerComponent();

    FSkillCastResult TryCastSkillAtIndex(int32 index);

    TWeakObjectPtr<UNewAbstractSkill> GetCurrentlyExecutedSkill() const { return _currentlyExecutedSkill; }

protected:
    void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Skills")
    TArray<TSubclassOf<UNewAbstractSkill>> _skillClasses{};

private:
    void _onCurrentlyExecutedSkillCastPhaseEnd(FSkillCastResult skillCastResult);
    void _onCurrentlyExecutedSkillChannelingPhaseEnd(FSkillChannelingResult skillChannelingResult);

    UPROPERTY()
    TArray<TObjectPtr<UNewAbstractSkill>> _skills{};

    TWeakObjectPtr<UNewAbstractSkill> _currentlyExecutedSkill = nullptr;
};
