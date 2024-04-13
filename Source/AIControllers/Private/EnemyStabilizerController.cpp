// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyStabilizerController.h"

#include "Algo/AnyOf.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HealthComponent.h"
#include "NewSkillsContainerComponent.h"
#include "NewStateComponent.h"
#include "ThermodynamicsInteractorComponent.h"

AEnemyStabilizerController::AEnemyStabilizerController() {
    _movementCommandC = CreateDefaultSubobject<UAIMovementCommandComponent>(TEXT("AIMovementCommandComponent"));
    _perceptionC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    _runEQSC = CreateDefaultSubobject<URunEQSComponent>(TEXT("RunEQSComponent"));
}

void AEnemyStabilizerController::_checkTargetAllyForAttachment() {
    const auto blackBoard = GetBlackboardComponent();
    if (const auto targetAlly = Cast<AActor>(blackBoard->GetValueAsObject(_bbTargetAllyKeyName)); IsValid(targetAlly)) {
        TArray<AActor*> pawnAttachedActors;
        targetAlly->GetAttachedActors(pawnAttachedActors);

        const auto foundInfluencerActor = Algo::AnyOf(
            pawnAttachedActors, [castInfluencerActor = _castInfluencerActor](AActor* const attachedActor) { return attachedActor->IsA(castInfluencerActor); });
        blackBoard->SetValueAsBool(_bbIsTargetAttachedToActor, foundInfluencerActor);
    }
}

void AEnemyStabilizerController::Tick(const float deltaTime) {
    Super::Tick(deltaTime);

    if (!_patrolC.IsValid()) {
        return;
    }

    if (const auto pawn = GetPawn(); IsValid(pawn)) {
        const auto blackBoard = GetBlackboardComponent();

        // Did we reach our current patrol objective?
        const auto pawnLocation = GetPawn()->GetActorLocation();
        if (_patrolC.IsValid() && pawnLocation.Equals(_patrolC->GetCurrentPatrolObjective(), _patrolC->GetPatrolObjectiveTolerance())) {
            _patrolC->UpdatePatrolObjective();

            blackBoard->SetValueAsVector(_bbPatrolObjectiveKeyName, _patrolC->GetCurrentPatrolObjective());
        }

        if (_thermodynamicC.IsValid() && _tempDmgHandlerC.IsValid()) {
            blackBoard->SetValueAsBool(_bbIAmColdKeyName, _thermodynamicC->GetTemperature() < _tempDmgHandlerC->GetMinComfortTemperature());
        }
    }

    if (_castInfluencerActor != nullptr) {
        _timeSinceInfluencerCheck += deltaTime;

        if (_timeSinceInfluencerCheck > _timeBetweenInfluencerChecks) {
            _checkTargetAllyForAttachment();
            _timeSinceInfluencerCheck = 0.0f;
        }
    }
}

void AEnemyStabilizerController::OnPossess(APawn* inPawn) {
    Super::OnPossess(inPawn);

    if (!IsValid(_behaviorTree)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): missing Behavior Tree Class"), *FString{__FUNCTION__});
        return;
    }

    if (!ensureMsgf(IsValid(_castInfluencerActor), TEXT("%s(): missing cast-influencing actor, this stabilizer won't cast"), *FString{__FUNCTION__})) {
        return;
    }

    RunBehaviorTree(_behaviorTree);

    const auto blackBoard = GetBlackboardComponent();

    // Making sure the controlled pawn has a patrolC and setting the first objective for the BB.
    if (const auto pawn = GetPawn(); IsValid(pawn)) {
        if (const auto healthC = pawn->FindComponentByClass<UHealthComponent>(); IsValid(healthC)) {
            healthC->OnDeath().AddUObject(this, &AEnemyStabilizerController::_onControlledPawnDeath);
        }

        _patrolC = pawn->FindComponentByClass<UPatrolComponent>();
        if (_patrolC.IsValid()) {
            _patrolC->StartPatrol();

            blackBoard->SetValueAsVector(_bbPatrolObjectiveKeyName, _patrolC->GetCurrentPatrolObjective());
        } else {
            UE_LOG(LogTemp, Error, TEXT("%s(): controlled pawn is missing Patrol Component"), *FString{__FUNCTION__});
        }

        _thermodynamicC = pawn->FindComponentByClass<UThermodynamicsInteractorComponent>();
        if (!_thermodynamicC.IsValid()) {
            UE_LOG(LogTemp, Error, TEXT("%s(): controlled pawn is missing Thermodynamic Component"), *FString{__FUNCTION__});
        }

        _tempDmgHandlerC = pawn->FindComponentByClass<UTemperatureDamageHandlerComponent>();
        if (!_tempDmgHandlerC.IsValid()) {
            UE_LOG(LogTemp, Error, TEXT("%s(): controlled pawn is missing Temperature Damage Handler Component"), *FString{__FUNCTION__});
        }
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): Controlled Pawn is unset"), *FString{__FUNCTION__});
    }

    // Setting up the logic that lets the BT know if the Target Ally has just changed or not.
    const auto targetEnemyKeyId = blackBoard->GetKeyID(_bbTargetAllyKeyName);
    if (targetEnemyKeyId != FBlackboard::InvalidKey) {
        _runEQSC->OnQueryResultChange().AddUObject(this, &AEnemyStabilizerController::_newOnTargetAllyChange);
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): Invald value for _bbTargetAllyKeyName"), *FString{__FUNCTION__});
    }

    _movementCommandC->SetDefaultMovementMode();

    if (IsValid(GetPawn())) {
        const auto skillsContainerC = GetPawn()->FindComponentByClass<UNewSkillsContainerComponent>();
        check(IsValid(skillsContainerC));
        skillsContainerC->OnSkillInExecutionStatusChanged().AddUObject(this, &AEnemyStabilizerController::_onSkillInExecutionStatusChanged);
    }
}

EBlackboardNotificationResult AEnemyStabilizerController::_onTargetAllyChange(const UBlackboardComponent& blackboard, const FBlackboard::FKey changedKeyID) {
    // I might be pushing my luck here...
    auto& nonConstBB = const_cast<UBlackboardComponent&>(blackboard);
    nonConstBB.SetValueAsBool(_bbTargetRecentlyChangedKeyName, true);

    _checkTargetAllyForAttachment();

    FTimerHandle handle;
    GetWorldTimerManager().SetTimer(
        handle,
        [&nonConstBB, bbTargetRecentlyChangedKeyName = _bbTargetRecentlyChangedKeyName]() { nonConstBB.SetValueAsBool(bbTargetRecentlyChangedKeyName, false); },
        _targetRecentlyChangedTimer, false);

    return EBlackboardNotificationResult::ContinueObserving;
}

void AEnemyStabilizerController::_newOnTargetAllyChange() {
    auto blackboardC = GetBlackboardComponent();
    blackboardC->SetValueAsBool(_bbTargetRecentlyChangedKeyName, true);

    _checkTargetAllyForAttachment();

    auto& timerManager = GetWorldTimerManager();
    timerManager.ClearTimer(_recentlyChangedHandle);
    timerManager.SetTimer(
        _recentlyChangedHandle,
        [blackboardC, bbTargetRecentlyChangedKeyName = _bbTargetRecentlyChangedKeyName]() {
            // The caster might be dead when the timer has expired.
            if (IsValid(blackboardC)) {
                blackboardC->SetValueAsBool(bbTargetRecentlyChangedKeyName, false);
            }
        },
        _targetRecentlyChangedTimer, false);
}

void AEnemyStabilizerController::_onSkillExecutionBegin() {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, true);
}

void AEnemyStabilizerController::_onSkillExecutionEnd() {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, false);
}

void AEnemyStabilizerController::_onSkillInExecutionStatusChanged(bool isExecutingSomeSkill) {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, isExecutingSomeSkill);
}

void AEnemyStabilizerController::_onControlledPawnDeath(const TObjectPtr<AActor> deadActor) {
    const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
    check(IsValid(stateC));

    stateC->TryAbort();
}
