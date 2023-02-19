// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CastSpell.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SkillsContainerComponent.h"
#include "StateComponent.h"



UBTTask_CastSpell::UBTTask_CastSpell() {
	NodeName = TEXT("Cast Spell");
}

EBTNodeResult::Type UBTTask_CastSpell::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
	if (!IsValid(_skillToCast)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): Skill to cast hasn't been selected"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	const auto aiController = ownerComp.GetAIOwner();
	const auto skillsContainerC = aiController->GetPawn()->FindComponentByClass<USkillsContainerComponent>();
	if (!IsValid(skillsContainerC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI-Controlled Pawn does not have a Skills Container Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	auto skill = skillsContainerC->FindSkillByClass(_skillToCast);
	if (!IsValid(skill)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI-Controlled Pawn does not have the selected Skill"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	const auto stateC = aiController->FindComponentByClass<UStateComponent>();
	if (!IsValid(stateC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AIController does not have a State Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	// The spell execution can fail, for example:
	// - In case the skill is on cooldown
	// - In case the skill is already being executed
	// - In case the current state does not allow the execution of skills
	return stateC->PerformSkillExecutionBehavior(MoveTemp(skill)) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
