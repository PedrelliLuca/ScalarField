// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"

#include "EnemyChaserController.generated.h"

/**
 * \brief A close-range mage that is obsessed with dealing damage to its enemies. They don't care of their own well-being.
 */
UCLASS()
class AICONTROLLERS_API AEnemyChaserController : public AAIController {
	GENERATED_BODY()

public:
	AEnemyChaserController();

	void Tick(float deltaSeconds) override;

protected:
	void BeginPlay() override;

private:
	void _updatePatrolObjective();
	
	UPROPERTY(EditDefaultsOnly,  Category = "Chaser | Behavior Tree")
	TObjectPtr<UBehaviorTree> _behaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Chaser | Patrolling Behavior")
	TArray<FVector> _patrolObjectives;

	UPROPERTY(EditDefaultsOnly, Category = "Chaser | Patrolling Behavior", meta = (ClampMin = "0"))
	int32 _startingPatrolObjectiveIdx = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Chaser | Patrolling Behavior", meta = (ClampMin = "0"))
	int32 _currentPatrolObjectiveIdx = 0;

	/** \brief Name of the Blackboard key that gets updated when a patrol objective is reached. */
	UPROPERTY(EditDefaultsOnly, Category = "Chaser | Patrolling Behavior")
	FName _bbPatrolObjectiveKeyName = "PatrolObjective";
};
