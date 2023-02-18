// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Tasks/AITask_MoveTo.h"

#include "BTTask_MoveToBycommand.generated.h"

struct FBTMoveToByCommandTaskMemory {
	/** Move request ID */
	FAIRequestID MoveRequestID;

	FDelegateHandle BBObserverDelegateHandle;
	FVector PreviousGoalLocation;

	TWeakObjectPtr<UAITask_MoveTo> Task;

	uint8 bObserverCanFinishTask : 1;
};

/**
 * 
 */
UCLASS(config=Game)
class SCALARFIELD_API UBTTask_MoveToByCommand : public UBTTask_BlackboardBase {
	GENERATED_BODY()

public:
	UBTTask_MoveToByCommand();
	
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

	void PostLoad() override;

#if WITH_EDITOR
	virtual void OnNodeCreated() override;
#endif 
	
	EBTNodeResult::Type _performMoveTask(UBehaviorTreeComponent& onwerComp, uint8* nodeMemory);

	EBlackboardNotificationResult _onBlackboardValueChange(const UBlackboardComponent& blackboardC, FBlackboard::FKey changedKeyID);

	/** if set, radius of goal's capsule will be added to threshold between AI and goal location in destination reach test  */
	UPROPERTY(Category = Node, EditAnywhere)
	uint32 _bReachTestIncludesGoalRadius : 1;
	
	/** if set, radius of AI's capsule will be added to threshold between AI and goal location in destination reach test  */
	UPROPERTY(Category = Node, EditAnywhere)
	uint32 _bReachTestIncludesAgentRadius : 1;

	/** if set, move will use pathfinding. Not exposed on purpose, please use BTTask_MoveDirectlyToward */
	uint32 _bUsePathfinding : 1;

	UPROPERTY(Category = Node, EditAnywhere)
	uint32 _bAllowStrafe : 1;

	/** if set, use incomplete path when goal can't be reached */
	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
	uint32 _bAllowPartialPath : 1;

	/** if set, path to goal actor will update itself when actor moves */
	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
	uint32 _bTrackMovingGoal : 1;

	/** if set, goal location will be projected on navigation data (navmesh) before using */
	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
	uint32 _bProjectGoalLocation : 1;

	/** if task is expected to react to changes to location represented by BB key 
	 *	this property can be used to tweak sensitivity of the mechanism. Value is 
	 *	recommended to be less than AcceptableRadius */
	UPROPERTY(Category=Blackboard, EditAnywhere, meta = (ClampMin = "1", UIMin = "1", EditCondition="bObserveBlackboardValue", DisplayAfter="bObserveBlackboardValue"))
	float _observedBlackboardValueTolerance;

	/** DEPRECATED, please use combination of bReachTestIncludes*Radius instead */
	UPROPERTY(Category = Node, VisibleInstanceOnly)
	uint32 _bStopOnOverlap : 1;
	
	UPROPERTY()
	uint32 _bStopOnOverlapNeedsUpdate : 1;

	/** if move goal in BB changes the move will be redirected to new location */
	UPROPERTY(Category = Blackboard, EditAnywhere)
	uint32 _bObserveBlackboardValue : 1;

	/** fixed distance added to threshold between AI and goal location in destination reach test */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0"))
	float _acceptableRadius;

	/** "None" will result in default filter being used */
	UPROPERTY(Category = Node, EditAnywhere)
	TSubclassOf<UNavigationQueryFilter> _filterClass;
};
