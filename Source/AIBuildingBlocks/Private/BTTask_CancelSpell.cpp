// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CancelSpell.h"

#include "AIController.h"
#include "NewStateComponent.h"

UBTTask_CancelSpell::UBTTask_CancelSpell() {
    NodeName = "Cancel Spell";
}

EBTNodeResult::Type UBTTask_CancelSpell::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    auto const pawn = ownerComp.GetAIOwner()->GetPawn();
    check(IsValid(pawn));

    auto const stateC = pawn->FindComponentByClass<UNewStateComponent>();
    if (!ensureAlwaysMsgf(IsValid(stateC), TEXT("AI-controlled Pawn does not have a State Component"))) {
        return EBTNodeResult::Failed;
    }

    stateC->TryAbort();
    return EBTNodeResult::Succeeded;
}
