// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_HasTargets.h"

bool UBTDecorator_HasTargets::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
    // TODO: retrieve RunEQSComponent, query it for targets, return true if any. Add option to return true only if targets recently changed.
    return true;
}

FString UBTDecorator_HasTargets::GetStaticDescription() const {
    // TODO
    return Super::GetStaticDescription();
}
