// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "AIMovementCommandComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/PatrolComponent.h"
#include "Components/RunEQSComponent.h"
#include "FactionComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "StateComponent.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicComponent.h"

#include "EnemyStabilizerController.generated.h"

/**
 * \brief A close-range mage that is obsessed with dealing damage to its enemies. They don't care of their own well-being.
 */
UCLASS()
class AICONTROLLERS_API AEnemyStabilizerController : public AAIController {
    GENERATED_BODY()

public:
    AEnemyStabilizerController();

    void Tick(float deltaTime) override;

protected:
    void BeginPlay() override;

private:
    EBlackboardNotificationResult _onTargetAllyChange(const UBlackboardComponent& blackboard, FBlackboard::FKey changedKeyID);
    void _newOnTargetAllyChange();

    void _checkTargetAllyForAttachment();

    void _onSkillExecutionBegin();
    void _onSkillExecutionEnd();

    void _onSkillInExecutionStatusChanged(bool isExecutingSomeSkill);

    void _onControlledPawnDeath(const TObjectPtr<AActor> deadActor);

    UPROPERTY(VisibleAnywhere, Category = "Stabilizer | Movement Commands")
    TObjectPtr<UAIMovementCommandComponent> _movementCommandC;

    UPROPERTY(VisibleAnywhere, Category = "Stabilizer | State Machine")
    TObjectPtr<UStateComponent> _stateC;

    UPROPERTY(VisibleAnywhere, Category = "Stabilizer | Perception")
    TObjectPtr<UAIPerceptionComponent> _perceptionC;

    UPROPERTY(VisibleAnywhere, Category = "Stabilizer | EQS")
    TObjectPtr<URunEQSComponent> _runEQSC;

    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Key Names")
    FName _bbTargetAllyKeyName = "TargetAlly";

    /** \brief Name of the Blackboard key that gets updated when a patrol objective is reached. */
    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Key Names")
    FName _bbPatrolObjectiveKeyName = "PatrolObjective";

    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Key Names")
    FName _bbIsCastingKeyName = "IsCasting";

    /** \brief Signals that the value of the BB key that keeps track of target has recently changed */
    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Key Names")
    FName _bbTargetRecentlyChangedKeyName = "TargetRecentlyChanged";

    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Key Names")
    FName _bbIAmColdKeyName = "IAmCold";

    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Key Names")
    FName _bbIsTargetAttachedToActor = "IsTargetAttachedToActor";

    /** \brief The attachment of the controlled pawn to this actor determines whether we can set the actor as target of our spell or not. */
    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Behavior-Influencing Properties")
    TSubclassOf<AActor> _castInfluencerActor = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Behavior-Influencing Properties")
    float _timeBetweenInfluencerChecks = 1.0f;

    /** \brief For how long we'll consider as recent a change in the BB key who signals a change of the target */
    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Blackboard")
    float _targetRecentlyChangedTimer = 0.1f;

    UPROPERTY(EditDefaultsOnly, Category = "Stabilizer | Behavior Tree")
    TObjectPtr<UBehaviorTree> _behaviorTree;

    UPROPERTY(EditDefaultsOnly, Category = "Feature Toggles")
    bool _bNewSkillSystem = false;

    float _timeSinceInfluencerCheck = 0.0f;

    // The controlled Pawn has ownership of these
    TWeakObjectPtr<UFactionComponent> _pawnFactionC = nullptr;
    TWeakObjectPtr<UPatrolComponent> _patrolC = nullptr;
    TWeakObjectPtr<UThermodynamicComponent> _thermodynamicC = nullptr;
    TWeakObjectPtr<UTemperatureDamageHandlerComponent> _tempDmgHandlerC = nullptr;

    FTimerHandle _recentlyChangedHandle;
};
