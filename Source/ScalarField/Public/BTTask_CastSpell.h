// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_CastSpell.generated.h"

/**
 *
 */
UCLASS()
class SCALARFIELD_API UBTTask_CastSpell : public UBTTaskNode {
    GENERATED_BODY()

public:
    UBTTask_CastSpell();

private:
    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

    FString GetStaticDescription() const override;

    UPROPERTY(EditAnywhere, Category = "Node")
    TSubclassOf<UAbstractSkill> _skillToCast;
};
