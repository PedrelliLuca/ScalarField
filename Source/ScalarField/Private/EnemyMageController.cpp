// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMageController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

AEnemyMageController::AEnemyMageController() {
	_perceptionC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AEnemyMageController::BeginPlay() {
	Super::BeginPlay();

	_perceptionC->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyMageController::_onActorSensed);
}

void AEnemyMageController::_onActorSensed(AActor* const actor, const FAIStimulus stimulus) {
	if (const auto character = Cast<ACharacter>(actor); IsValid(character) && character->IsPlayerControlled()) {
		const auto blackBoard = GetBlackboardComponent();
		blackBoard->SetValueAsBool(_canSeeKeyName, stimulus.WasSuccessfullySensed());
		blackBoard->SetValueAsObject(_targetKeyName, character);
	}
}
