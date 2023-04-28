// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "StateComponent.h"

#include "BTTask_CastSpellOnTarget.generated.h"

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UBTTask_CastSpellOnTarget : public UBTTask_BlackboardBase {
    GENERATED_BODY()

public:
    UBTTask_CastSpellOnTarget();

private:
    EBTNodeResult::Type _determineCastSuccessWithTarget(
        const UBehaviorTreeComponent& ownerComp, const TObjectPtr<UStateComponent>& stateC, const TObjectPtr<UAbstractSkill>& skill);
    EBTNodeResult::Type _determineCastSuccessWithoutTarget(const TObjectPtr<UStateComponent>& stateC, const TObjectPtr<UAbstractSkill>& skill);
    bool _isManaAvailableForSkill(const TObjectPtr<AAIController>& aiController, const TObjectPtr<UAbstractSkill>& skill);

    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

    FString GetStaticDescription() const override;

    UPROPERTY(EditAnywhere, Category = "Node")
    TSubclassOf<UAbstractSkill> _skillToCast;

    /** \brief If true, the cast of the spell does not start if the mana necessary to both cast and channel the skill isn't immediately available. If false, the
     * cast is always tried.
     * "True" is generally preferred. However, for casters with a lot of mana regen, some skills might be worth casting even if not all the mana is immediately
     * available, because it will be by the end of the cast. */
    UPROPERTY(EditAnywhere, Category = "Cast Spell")
    bool _needsManaAvailabilityToCast = true;
};
