// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
#include "BTTask_GetTargetPosition.generated.h"

/**
 *
 */
UCLASS()
class SCALARFIELD_API UBTTask_GetTargetPosition : public UBTTaskNode {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = Blackboard)
    FBlackboardKeySelector TargetSelector{};

    UPROPERTY(EditAnywhere, Category = Blackboard)
    FBlackboardKeySelector PositionSelector{};

    UPROPERTY(EditAnywhere, Category = Blackboard)
    bool RandomizeInRadius = true;

    UPROPERTY(EditAnywhere, Category = Blackboard)
    float Radius = 100.0f;

private:
    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};
