// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "AIMovementCommandComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "FactionComponent.h"
#include "Components/PatrolComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "StateComponent.h"

#include "EnemyChaserController.generated.h"

/**
 * \brief A close-range mage that is obsessed with dealing damage to its enemies. They don't care of their own well-being.
 */
UCLASS()
class AICONTROLLERS_API AEnemyChaserController : public AAIController {
	GENERATED_BODY()

public:
	AEnemyChaserController();

	void Tick(float deltaTime) override;

protected:
	void BeginPlay() override;

private:
	UFUNCTION()
	void _onActorSensed(AActor* actor, FAIStimulus stimulus);

	UPROPERTY(VisibleAnywhere, Category = "Chaser | Movement Commands")
	TObjectPtr<UAIMovementCommandComponent> _movementCommandC;

	UPROPERTY(VisibleAnywhere, Category = "Chaser | State Machine")
	TObjectPtr<UStateComponent> _stateC;

	UPROPERTY(VisibleAnywhere, Category = "Chaser | Perception")
	TObjectPtr<UAIPerceptionComponent> _perceptionC;

	UPROPERTY(EditDefaultsOnly, Category = "Chaser | Perception")
	FName _bbCanSeeEnemyKeyName = "CanSeeEnemy";

	UPROPERTY(EditDefaultsOnly, Category = "Chaser | Perception")
	FName _bbTargetEnemyKeyName = "TargetEnemy";
	
	UPROPERTY(EditDefaultsOnly,  Category = "Chaser | Behavior Tree")
	TObjectPtr<UBehaviorTree> _behaviorTree;

	/** \brief Name of the Blackboard key that gets updated when a patrol objective is reached. */
	UPROPERTY(EditDefaultsOnly, Category = "Chaser | Patrolling Behavior")
	FName _bbPatrolObjectiveKeyName = "PatrolObjective";

	// The controlled Pawn has ownership of these
	TWeakObjectPtr<UFactionComponent> _pawnFactionC = nullptr;
	TWeakObjectPtr<UPatrolComponent> _patrolC = nullptr;
};
