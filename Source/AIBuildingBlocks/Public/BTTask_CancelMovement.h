// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_CancelMovement.generated.h"

/**
 * 
 */
UCLASS()
class AIBUILDINGBLOCKS_API UBTTask_CancelMovement : public UBTTaskNode {
	GENERATED_BODY()

public:
	UBTTask_CancelMovement();
	
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

};
