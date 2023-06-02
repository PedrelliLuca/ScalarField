// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"
#include "BTDecorator_HasTargets.generated.h"

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UBTDecorator_HasTargets : public UBTDecorator {
    GENERATED_BODY()

public:

protected:
    bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
    FString GetStaticDescription() const override;

};
