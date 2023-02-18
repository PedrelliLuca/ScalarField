// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToByCommand.h"

#include "AIMovementCommandComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_MoveToByCommand::UBTTask_MoveToByCommand() {
	NodeName = "Move To By Command";

	// Accept only actors and vectors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToByCommand, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToByCommand, BlackboardKey));
}

EBTNodeResult::Type UBTTask_MoveToByCommand::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
	const auto aiController = ownerComp.GetAIOwner();
	const auto aiMovementCommandC = aiController->FindComponentByClass<UAIMovementCommandComponent>();
	if (!IsValid(aiMovementCommandC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI Controller does not have a Movement Command Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}


	const auto blackboardC = ownerComp.GetBlackboardComponent();

	FVector targetLocation{};
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
		targetLocation = blackboardC->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
		
	} else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
		const auto keyValue = blackboardC->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		const auto targetActor = Cast<AActor>(keyValue);
		check(IsValid(targetActor));
		targetLocation = targetActor->GetActorLocation();
	} else {
		checkNoEntry();
	}

	aiMovementCommandC->GetMovementCommand()->OnSetDestination(aiController, targetLocation);

	return EBTNodeResult::Succeeded;
}
