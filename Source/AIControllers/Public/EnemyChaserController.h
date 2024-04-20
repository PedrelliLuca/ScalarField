// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "AIMovementCommandComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/PatrolComponent.h"
#include "FactionComponent.h"
#include "Perception/AIPerceptionComponent.h"

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
    void OnPossess(APawn* inPawn) override;

private:
    EBlackboardNotificationResult _onTargetEnemyChange(const UBlackboardComponent& blackboard, FBlackboard::FKey changedKeyID);

    void _onSkillExecutionBegin();
    void _onSkillExecutionEnd();

    void _onSkillInExecutionStatusChanged(bool isExeuctingSomeSkill);

    void _onControlledPawnDeath(const TObjectPtr<AActor> deadActor);

    UPROPERTY(VisibleAnywhere, Category = "Chaser | Movement Commands")
    TObjectPtr<UAIMovementCommandComponent> _movementCommandC;

    UPROPERTY(VisibleAnywhere, Category = "Chaser | Perception")
    TObjectPtr<UAIPerceptionComponent> _perceptionC;

    UPROPERTY(EditDefaultsOnly, Category = "Chaser | Key Names")
    FName _bbTargetEnemyKeyName = "TargetEnemy";

    /** \brief Name of the Blackboard key that gets updated when a patrol objective is reached. */
    UPROPERTY(EditDefaultsOnly, Category = "Chaser | Key Names")
    FName _bbPatrolObjectiveKeyName = "PatrolObjective";

    UPROPERTY(EditDefaultsOnly, Category = "Chaser | Key Names")
    FName _bbIsCastingKeyName = "IsCasting";

    /** \brief Signals that the value of the BB key that keeps track of target has recently changed */
    UPROPERTY(EditDefaultsOnly, Category = "Chaser | Key Names")
    FName _bbTargetRecentlyChangedKeyName = "TargetRecentlyChanged";

    /** \brief For how long we'll consider as recent a change in the BB key who signals a change of the target */
    UPROPERTY(EditDefaultsOnly, Category = "Chaser | Blackboard")
    float _targetRecentlyChangedTimer = 0.1f;

    UPROPERTY(EditDefaultsOnly, Category = "Chaser | Behavior Tree")
    TObjectPtr<UBehaviorTree> _behaviorTree;

    // The controlled Pawn has ownership of these
    TWeakObjectPtr<UFactionComponent> _pawnFactionC = nullptr;
    TWeakObjectPtr<UPatrolComponent> _patrolC = nullptr;
};
