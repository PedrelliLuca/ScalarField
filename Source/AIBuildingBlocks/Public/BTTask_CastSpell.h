// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "NewAbstractSkill.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_CastSpell.generated.h"

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UBTTask_CastSpell : public UBTTaskNode {
    GENERATED_BODY()

public:
    UBTTask_CastSpell();

private:
    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

    EBTNodeResult::Type _executeTaskLegacy(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
    EBTNodeResult::Type _executeTaskNew(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);

    FString GetStaticDescription() const override;

    bool _isManaAvailableForSkill(const TObjectPtr<AAIController>& aiController, const TObjectPtr<UAbstractSkill>& skill);

    UPROPERTY(EditAnywhere, Category = "Cast Spell")
    TSubclassOf<UAbstractSkill> _skillToCast;

    UPROPERTY(EditAnywhere, Category = "Cast Spell")
    TSubclassOf<UNewAbstractSkill> _newSkillToCast;

    /** \brief If true, the cast of the spell does not start if the mana necessary to both cast and channel the skill isn't immediately available. If false, the
     * cast is always tried.
     * "True" is generally preferred. However, for casters with a lot of mana regen, some skills might be worth casting even if not all the mana is immediately
     * available, because it will be by the end of the cast. */
    UPROPERTY(EditAnywhere, Category = "Cast Spell")
    bool _needsManaAvailabilityToCast = true;

    UPROPERTY(EditInstanceOnly, Category = "Feature Toggles")
    bool _bNewSkillSystem = false;
};
