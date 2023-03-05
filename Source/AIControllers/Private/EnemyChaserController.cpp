// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChaserController.h"

#include "BehaviorTree/BlackboardComponent.h"

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

AEnemyChaserController::AEnemyChaserController() {
}

void AEnemyChaserController::BeginPlay() {
	Super::BeginPlay();
	
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
