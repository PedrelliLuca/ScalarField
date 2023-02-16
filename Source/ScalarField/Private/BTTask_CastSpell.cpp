// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CastSpell.h"

#include "AbstractSkill.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SkillsContainerComponent.h"
#include "StateComponent.h"



UBTTask_CastSpell::UBTTask_CastSpell() {
	NodeName = TEXT("Cast Spell");

	// Accept only classes
	BlackboardKey.AddClassFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CastSpell, BlackboardKey), TSubclassOf<UClass>(UAbstractSkill::StaticClass()));
}

EBTNodeResult::Type UBTTask_CastSpell::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
	check(BlackboardKey.SelectedKeyType == UBlackboardKeyType_Class::StaticClass());

	const auto aiController = ownerComp.GetAIOwner();
	const auto skillsContainerC = aiController->GetPawn()->FindComponentByClass<USkillsContainerComponent>();
	if (!IsValid(skillsContainerC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI-Controlled Pawn does not have a Skills Container Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	const auto blackboard = ownerComp.GetBlackboardComponent();
	const auto spellClass = blackboard->GetValue<UBlackboardKeyType_Class>(BlackboardKey.GetSelectedKeyID());
	auto skill = skillsContainerC->FindSkillByClass(spellClass);
	if (!IsValid(skill)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI-Controlled Pawn does not have the selected Skill"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	const auto stateC = aiController->FindComponentByClass<UStateComponent>();
	if (!IsValid(stateC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AIController does not have a State Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	return stateC->PerformSkillExecutionBehavior(MoveTemp(skill)) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
