// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_CancelSpell.generated.h"

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UBTTask_CancelSpell : public UBTTaskNode {
    GENERATED_BODY()

public:
    UBTTask_CancelSpell();

private:
    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};