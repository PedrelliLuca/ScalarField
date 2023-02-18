// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToByCommand.h"

#include "AIMovementCommandComponent.h"
#include "AISystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "VisualLogger/VisualLogger.h"


UBTTask_MoveToByCommand::UBTTask_MoveToByCommand() {
	NodeName = "Move To By Command";
	INIT_TASK_NODE_NOTIFY_FLAGS();

	_acceptableRadius = GET_AI_CONFIG_VAR(AcceptanceRadius);
	_bReachTestIncludesGoalRadius = _bReachTestIncludesAgentRadius = GET_AI_CONFIG_VAR(bFinishMoveOnGoalOverlap);
	_bAllowStrafe = GET_AI_CONFIG_VAR(bAllowStrafing);
	_bAllowPartialPath = GET_AI_CONFIG_VAR(bAcceptPartialPaths);
	_bTrackMovingGoal = true;
	_bProjectGoalLocation = true;
	_bUsePathfinding = true;

	_bStopOnOverlapNeedsUpdate = true;

	_observedBlackboardValueTolerance = _acceptableRadius * 0.95f;

	// Only Actors and Vectors are accepted
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToByCommand, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToByCommand, BlackboardKey));
}

EBTNodeResult::Type UBTTask_MoveToByCommand::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
	const auto memory = CastInstanceNodeMemory<FBTMoveToByCommandTaskMemory>(nodeMemory);
	memory->PreviousGoalLocation = FAISystem::InvalidLocation;
	memory->MoveRequestID = FAIRequestID::InvalidRequest;

	const auto aiController = ownerComp.GetAIOwner();
	check(IsValid(aiController));

	auto nodeResult = _performMoveTask(ownerComp, nodeMemory);

	
}

void UBTTask_MoveToByCommand::PostLoad() {
	Super::PostLoad();

	if (_bStopOnOverlapNeedsUpdate) {
		_bStopOnOverlapNeedsUpdate = false;
		_bReachTestIncludesAgentRadius = _bStopOnOverlap;
		_bReachTestIncludesGoalRadius = false;
	}
}

#if WITH_EDITOR

void UBTTask_MoveToByCommand::OnNodeCreated() {
	Super::OnNodeCreated();
	// _bStopOnOverlapNeedsUpdate = false;
}

#endif

EBTNodeResult::Type UBTTask_MoveToByCommand::_performMoveTask(UBehaviorTreeComponent& onwerComp, uint8* nodeMemory) {
	/*const auto aiMovementCommandC = aiController->FindComponentByClass<UAIMovementCommandComponent>();
	if (!IsValid(aiMovementCommandC)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI Controller does not have a Movement Command Component"), *FString{ __FUNCTION__ });
		return EBTNodeResult::Failed;
	}

	check(BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass());

	const auto blackboardC = ownerComp.GetBlackboardComponent();
	const FVector targetLocation = blackboardC->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	aiMovementCommandC->GetMovementCommand()->OnStopMovement(aiController);
	aiMovementCommandC->GetMovementCommand()->OnSetDestination(aiController, targetLocation);

	return EBTNodeResult::Succeeded;*/
	
	auto nodeResult = EBTNodeResult::Failed;

	const auto blackboardC = onwerComp.GetBlackboardComponent();
	const auto aiController = onwerComp.GetAIOwner();

	if (!IsValid(aiController) || !IsValid(blackboardC)) {
		return nodeResult;
	}

	// Move Request preparation
	FAIMoveRequest moveReq;
	moveReq.SetNavigationFilter(*_filterClass ? _filterClass : aiController->GetDefaultNavigationFilterClass());
	moveReq.SetAllowPartialPath(_bAllowPartialPath);
	moveReq.SetAcceptanceRadius(_acceptableRadius);
	moveReq.SetCanStrafe(_bAllowStrafe);
	moveReq.SetReachTestIncludesAgentRadius(_bReachTestIncludesAgentRadius);
	moveReq.SetReachTestIncludesGoalRadius(_bReachTestIncludesGoalRadius);
	moveReq.SetProjectGoalLocation(_bProjectGoalLocation);
	moveReq.SetUsePathfinding(_bUsePathfinding);

	const auto myMemory = CastInstanceNodeMemory<FBTMoveToByCommandTaskMemory>(nodeMemory);

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
		const auto keyValue = blackboardC->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		const auto targetActor = Cast<AActor>(keyValue);
		if (IsValid(targetActor)) {
			_bTrackMovingGoal ? moveReq.SetGoalActor(targetActor) : moveReq.SetGoalLocation(targetActor->GetActorLocation());
		} else {
			UE_VLOG(aiController, LogBehaviorTree, Warning, TEXT("%s(): Tried to go to actor while BB %s entry was empty"), *FString{__FUNCTION__}, *BlackboardKey.SelectedKeyName.ToString());
		}
	} else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
		const auto targetLocation = blackboardC->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
		moveReq.SetGoalLocation(targetLocation);
		myMemory->PreviousGoalLocation = targetLocation;
	} else {
		// Constructor says only Actors and Vectors are accepted, something horrible is happening
		checkNoEntry();
	}
}
