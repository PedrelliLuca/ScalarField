// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyStabilizerController.h"

#include "Algo/AnyOf.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyStabilizerController::AEnemyStabilizerController() {
    _movementCommandC = CreateDefaultSubobject<UAIMovementCommandComponent>(TEXT("AIMovementCommandComponent"));
    _stateC = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
    _perceptionC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AEnemyStabilizerController::Tick(float deltaTime) {
    Super::Tick(deltaTime);

    if (!_patrolC.IsValid()) {
        return;
    }

    _stateC->PerformTickBehavior(deltaTime);
    _movementCommandC->GetMovementCommand()->OnMovementTick(this, deltaTime);

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

        // This is quite bad performance-wise, I should find another way
        if (_castInfluencerActor != nullptr) {
            TArray<AActor*> pawnAttachedActors;
            pawn->GetAttachedActors(pawnAttachedActors);
            const auto foundInfluencerActor = Algo::AnyOf(pawnAttachedActors,
                [castInfluencerActor = _castInfluencerActor](AActor* const attachedActor) { return attachedActor->IsA(castInfluencerActor); });
            blackBoard->SetValueAsBool(_bbIsTargetAttachedToActor, foundInfluencerActor);
        }
    }
}

void AEnemyStabilizerController::BeginPlay() {
    Super::BeginPlay();

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
        _patrolC = pawn->FindComponentByClass<UPatrolComponent>();
        if (_patrolC.IsValid()) {
            _patrolC->StartPatrol();

            blackBoard->SetValueAsVector(_bbPatrolObjectiveKeyName, _patrolC->GetCurrentPatrolObjective());
        } else {
            UE_LOG(LogTemp, Error, TEXT("%s(): controlled pawn is missing Patrol Component"), *FString{__FUNCTION__});
        }

        _thermodynamicC = pawn->FindComponentByClass<UThermodynamicComponent>();
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
        blackBoard->RegisterObserver(
            targetEnemyKeyId, this, FOnBlackboardChangeNotification::CreateUObject(this, &AEnemyStabilizerController::_onTargetAllyChange));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): Invald value for _bbTargetAllyKeyName"), *FString{__FUNCTION__});
    }

    // Setting up the logic that lets the BT know if we're moving or not.
    _movementCommandC->OnActiveMovementCmdStateChanged().AddUObject(this, &AEnemyStabilizerController::_updateBlackboardOnMovementStatus);

    // Setting up the logic that lets the BT know if we're casting or not.
    _stateC->OnSkillExecutionBegin().AddUObject(this, &AEnemyStabilizerController::_onSkillExecutionBegin);
    _stateC->OnSkillExecutionEnd().AddUObject(this, &AEnemyStabilizerController::_onSkillExecutionEnd);
}

void AEnemyStabilizerController::_updateBlackboardOnMovementStatus(const bool newIsMoving) {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsMovingKeyName, newIsMoving);
}

EBlackboardNotificationResult AEnemyStabilizerController::_onTargetAllyChange(const UBlackboardComponent& blackboard, const FBlackboard::FKey changedKeyID) {
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

void AEnemyStabilizerController::_onSkillExecutionBegin() {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, true);
}

void AEnemyStabilizerController::_onSkillExecutionEnd() {
    const auto blackBoard = GetBlackboardComponent();
    blackBoard->SetValueAsBool(_bbIsCastingKeyName, false);
}
