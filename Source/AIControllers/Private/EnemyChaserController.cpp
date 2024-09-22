// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyChaserController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "HealthComponent.h"
#include "NewStateComponent.h"
#include "SkillsContainerComponent.h"

AEnemyChaserController::AEnemyChaserController() {
    _movementCommandC = CreateDefaultSubobject<UAIMovementCommandComponent>(TEXT("AIMovementCommandComponent"));
    _perceptionC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AEnemyChaserController::Tick(float const deltaTime) {
    Super::Tick(deltaTime);

    if (!_patrolC.IsValid()) {
        return;
    }

    // Did we reach our current patrol objective?
    if (auto const pawn = GetPawn(); IsValid(pawn)) {
        auto const pawnLocation = GetPawn()->GetActorLocation();
        if (pawnLocation.Equals(_patrolC->GetCurrentPatrolObjective(), _patrolC->GetPatrolObjectiveTolerance())) {
            _patrolC->UpdatePatrolObjective();

            auto const blackBoard = GetBlackboardComponent();
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

    auto const blackBoard = GetBlackboardComponent();

    // Making sure the controlled pawn has a patrolC and setting the first objective for the BB.
    if (auto const pawn = GetPawn(); IsValid(pawn)) {
        if (auto const healthC = pawn->FindComponentByClass<UHealthComponent>(); IsValid(healthC)) {
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
    auto const targetEnemyKeyId = blackBoard->GetKeyID(_bbTargetEnemyKeyName);
    if (targetEnemyKeyId != FBlackboard::InvalidKey) {
        blackBoard->RegisterObserver(
            targetEnemyKeyId, this, FOnBlackboardChangeNotification::CreateUObject(this, &AEnemyChaserController::_onTargetEnemyChange));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): Invald value for _bbTargetEnemyKeyName"), *FString{__FUNCTION__});
    }

    _movementCommandC->SetDefaultMovementMode();

    if (IsValid(GetPawn())) {
        auto const skillsContainerC = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainerC));

        skillsContainerC->CreateAllSkills();
        skillsContainerC->OnSkillInExecutionStatusChanged().AddUObject(this, &AEnemyChaserController::_onSkillInExecutionStatusChanged);
    }
}

EBlackboardNotificationResult AEnemyChaserController::_onTargetEnemyChange(UBlackboardComponent const& blackboard, FBlackboard::FKey const changedKeyID) {
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
    auto const blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, true);
}

void AEnemyChaserController::_onSkillExecutionEnd() {
    auto const blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, false);
}

void AEnemyChaserController::_onSkillInExecutionStatusChanged(bool const isExecutingSomeSkill) {
    auto const blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, isExecutingSomeSkill);
}

void AEnemyChaserController::_onControlledPawnDeath(TObjectPtr<AActor> const deadActor) {
    auto const stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
    check(IsValid(stateC));

    stateC->TryAbort();
}
