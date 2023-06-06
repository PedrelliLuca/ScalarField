// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"
#include "BTDecorator_TargetsToHave.generated.h"

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UBTDecorator_TargetsToHave : public UBTDecorator {
    GENERATED_BODY()

protected:
    bool CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const override;

private:
    FString GetStaticDescription() const override;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
    int32 _requiredTargets = 1;
};
