// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToByCommand.h"

#include "AIMovementCommandComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToByCommand::UBTTask_MoveToByCommand() {
	NodeName = "Move To By Command";

	// Accept only actors and vectors
	// BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToByCommand, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToByCommand, BlackboardKey));
}

EBTNodeResult::Type UBTTask_MoveToByCommand::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
	const auto aiController = ownerComp.GetAIOwner();
	const auto aiMovementCommandC = aiController->FindComponentByClass<UAIMovementCommandComponent>();
	if (!IsValid(aiMovementCommandC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI Controller does not have a Movement Command Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	check(BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass());

	const auto blackboardC = ownerComp.GetBlackboardComponent();
	const FVector targetLocation = blackboardC->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	aiMovementCommandC->GetMovementCommand()->OnStopMovement(aiController);
	aiMovementCommandC->GetMovementCommand()->OnSetDestination(aiController, targetLocation);

	return EBTNodeResult::Succeeded;
}
