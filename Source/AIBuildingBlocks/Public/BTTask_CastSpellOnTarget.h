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
    EBTNodeResult::Type _determineCastSuccessWithTarget(const UBehaviorTreeComponent& ownerComp, const TObjectPtr<UStateComponent>& stateC, const TObjectPtr<UAbstractSkill>& skill);
    EBTNodeResult::Type _determineCastSuccessWithoutTarget(const TObjectPtr<UStateComponent>& stateC, const TObjectPtr<UAbstractSkill>& skill);

    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

    FString GetStaticDescription() const override;

    UPROPERTY(EditAnywhere, Category = "Node")
    TSubclassOf<UAbstractSkill> _skillToCast;
};
