// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CancelSpell.h"

#include "AIController.h"
#include "SkillsContainerComponent.h"
#include "StateComponent.h"

UBTTask_CancelSpell::UBTTask_CancelSpell() {
    NodeName = "Cancel Spell";
}

EBTNodeResult::Type UBTTask_CancelSpell::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
	const auto aiController = ownerComp.GetAIOwner();
	const auto skillsContainerC = aiController->GetPawn()->FindComponentByClass<USkillsContainerComponent>();
	if (!IsValid(skillsContainerC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI-Controlled Pawn does not have a Skills Container Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	const auto stateC = aiController->FindComponentByClass<UStateComponent>();
	if (!IsValid(stateC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AIController does not have a State Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	stateC->PerformAbortBehavior();
	return EBTNodeResult::Succeeded;
}
