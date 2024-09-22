// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_TargetsToHave.h"

#include "AIController.h"
#include "Components/RunEQSComponent.h"

bool UBTDecorator_TargetsToHave::CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* const nodeMemory) const {
    auto const aiController = ownerComp.GetAIOwner();
    check(IsValid(aiController));

    auto const runEQSC = aiController->FindComponentByClass<URunEQSComponent>();
    if (!ensureMsgf(IsValid(runEQSC), TEXT("AI-Controlled pawn does not have a Run EQS Component"))) {
        return false;
    }

    return runEQSC->DidQueryReturnItems(_requiredTargets);
}

FString UBTDecorator_TargetsToHave::GetStaticDescription() const {
    // TODO
    return Super::GetStaticDescription();
}
