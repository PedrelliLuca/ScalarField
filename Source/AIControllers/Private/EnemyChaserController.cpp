// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyChaserController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "HealthComponent.h"
#include "NewSkillsContainerComponent.h"
#include "NewStateComponent.h"

AEnemyChaserController::AEnemyChaserController() {
    _movementCommandC = CreateDefaultSubobject<UAIMovementCommandComponent>(TEXT("AIMovementCommandComponent"));
    _stateC = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
    _perceptionC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AEnemyChaserController::Tick(const float deltaTime) {
    Super::Tick(deltaTime);

    if (!_patrolC.IsValid()) {
        return;
    }

    if (!_bNewSkillSystem) {
        _stateC->PerformTickBehavior(deltaTime);
        _movementCommandC->MovementTick(deltaTime);
    }

    // Did we reach our current patrol objective?
    if (const auto pawn = GetPawn(); IsValid(pawn)) {
        const auto pawnLocation = GetPawn()->GetActorLocation();
        if (pawnLocation.Equals(_patrolC->GetCurrentPatrolObjective(), _patrolC->GetPatrolObjectiveTolerance())) {
            _patrolC->UpdatePatrolObjective();

            const auto blackBoard = GetBlackboardComponent();
            blackBoard->SetValueAsVector(_bbPatrolObjectiveKeyName, _patrolC->GetCurrentPatrolObjective());
        }
    }
}

void AEnemyChaserController::OnPossess(APawn* const inPawn) {
    Super::OnPossess(inPawn);

    if (!IsValid(_behaviorTree)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): missing Behavior Tree Class"), *FString{__FUNCTION__});
        return;
    }

    RunBehaviorTree(_behaviorTree);

    const auto blackBoard = GetBlackboardComponent();

    // Making sure the controlled pawn has a patrolC and setting the first objective for the BB.
    if (const auto pawn = GetPawn(); IsValid(pawn)) {
        if (const auto healthC = pawn->FindComponentByClass<UHealthComponent>(); IsValid(healthC)) {
            healthC->OnDeath().AddUObject(this, &AEnemyChaserController::_onControlledPawnDeath);
        }

        _patrolC = pawn->FindComponentByClass<UPatrolComponent>();
        if (_patrolC.IsValid()) {
            _patrolC->StartPatrol();

            blackBoard->SetValueAsVector(_bbPatrolObjectiveKeyName, _patrolC->GetCurrentPatrolObjective());
        } else {
            UE_LOG(LogTemp, Error, TEXT("%s(): controlled pawn is missing Patrol Component"), *FString{__FUNCTION__});
        }
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): Controlled Pawn is unset"), *FString{__FUNCTION__});
    }

    // Setting up the logic that lets the BT know if the Target Enemy has just changed or not.
    const auto targetEnemyKeyId = blackBoard->GetKeyID(_bbTargetEnemyKeyName);
    if (targetEnemyKeyId != FBlackboard::InvalidKey) {
        blackBoard->RegisterObserver(
            targetEnemyKeyId, this, FOnBlackboardChangeNotification::CreateUObject(this, &AEnemyChaserController::_onTargetEnemyChange));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): Invald value for _bbTargetEnemyKeyName"), *FString{__FUNCTION__});
    }

    if (_bNewSkillSystem) {
        _movementCommandC->SetDefaultMovementMode();
    }

    if (!_bNewSkillSystem) {
        // Setting up the logic that lets the BT know if we're casting or not.
        _stateC->OnSkillExecutionBegin().AddUObject(this, &AEnemyChaserController::_onSkillExecutionBegin);
        _stateC->OnSkillExecutionEnd().AddUObject(this, &AEnemyChaserController::_onSkillExecutionEnd);
    } else {
        if (IsValid(GetPawn())) {
            const auto skillsContainerC = GetPawn()->FindComponentByClass<UNewSkillsContainerComponent>();
            check(IsValid(skillsContainerC));
            skillsContainerC->OnSkillInExecutionStatusChanged().AddUObject(this, &AEnemyChaserController::_onSkillInExecutionStatusChanged);
        }
    }
}

EBlackboardNotificationResult AEnemyChaserController::_onTargetEnemyChange(const UBlackboardComponent& blackboard, const FBlackboard::FKey changedKeyID) {
    // I might be pushing my luck here...
    auto& nonConstBB = const_cast<UBlackboardComponent&>(blackboard);
    nonConstBB.SetValueAsBool(_bbTargetRecentlyChangedKeyName, true);

    FTimerHandle handle;
    GetWorldTimerManager().SetTimer(
        handle,
        [&nonConstBB, bbTargetRecentlyChangedKeyName = _bbTargetRecentlyChangedKeyName]() { nonConstBB.SetValueAsBool(bbTargetRecentlyChangedKeyName, false); },
        _targetRecentlyChangedTimer, false);

    return EBlackboardNotificationResult::ContinueObserving;
}

void AEnemyChaserController::_onSkillExecutionBegin() {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, true);
}

void AEnemyChaserController::_onSkillExecutionEnd() {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, false);
}

void AEnemyChaserController::_onSkillInExecutionStatusChanged(const bool isExecutingSomeSkill) {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, isExecutingSomeSkill);
}

void AEnemyChaserController::_onControlledPawnDeath(const TObjectPtr<AActor> deadActor) {
    if (!_bNewSkillSystem) {
        _stateC->PerformAbortBehavior();
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryAbort();
    }
}
