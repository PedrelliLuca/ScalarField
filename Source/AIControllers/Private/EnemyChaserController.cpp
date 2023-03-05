// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChaserController.h"

#include "BehaviorTree/BlackboardComponent.h"

AEnemyChaserController::AEnemyChaserController() {
	_perceptionC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AEnemyChaserController::Tick(const float deltaSeconds) {
	Super::Tick(deltaSeconds);

	// Did we reach our current patrol objective?
	if (const auto pawn = GetPawn(); IsValid(pawn)) {
		const auto pawnLocation = GetPawn()->GetActorLocation();
		if (pawnLocation.Equals(_patrolObjectives[_currentPatrolObjectiveIdx], 50)) {
			_updatePatrolObjective();
		}
	}
}

void AEnemyChaserController::BeginPlay() {
	Super::BeginPlay();

	if (const auto pawn = GetPawn(); IsValid(pawn)) {
		_pawnFactionC = pawn->FindComponentByClass<UFactionComponent>();
		if (_pawnFactionC.IsValid()) {
			_perceptionC->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyChaserController::_onActorSensed);
		} else {
			UE_LOG(LogTemp, Error, TEXT("%s(): controlled pawn is missing Faction Component"), *FString{__FUNCTION__});
		}	
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s(): Controlled Pawn is unset"), *FString{__FUNCTION__});
	}
	
	if(!IsValid(_behaviorTree)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): missing Behavior Tree Class"), *FString{__FUNCTION__});
		return;
	}
	
	RunBehaviorTree(_behaviorTree);

	const auto blackBoard = GetBlackboardComponent();
	blackBoard->SetValueAsVector(_bbPatrolObjectiveKeyName, _patrolObjectives[_startingPatrolObjectiveIdx]);
	_currentPatrolObjectiveIdx = _startingPatrolObjectiveIdx;
}

void AEnemyChaserController::_updatePatrolObjective() {
	// Traversing the array as a circle
	_currentPatrolObjectiveIdx = (_currentPatrolObjectiveIdx + 1) % _patrolObjectives.Num();

	const auto blackBoard = GetBlackboardComponent();
	blackBoard->SetValueAsVector(_bbPatrolObjectiveKeyName, _patrolObjectives[_currentPatrolObjectiveIdx]);
}

void AEnemyChaserController::_onActorSensed(AActor* const actor, const FAIStimulus stimulus) {
	// Did we sense an actor belonging to some faction? If not, we consider it neutral and ignore it.
	if (const auto factionC = actor->FindComponentByClass<UFactionComponent>()) {
		check(_pawnFactionC.IsValid());

		// Is this actor an enemy? Chasers only care about enemies.
		if (_pawnFactionC->IsEnemyWithFaction(factionC->GetFaction())) {
			const auto blackBoard = GetBlackboardComponent();
			blackBoard->SetValueAsBool(_bbCanSeeEnemyKeyName, stimulus.WasSuccessfullySensed());
			blackBoard->SetValueAsObject(_bbTargetEnemyKeyName, actor);
		}
	}
}
