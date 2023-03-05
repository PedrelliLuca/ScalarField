// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMageControllerCopy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

AEnemyMageControllerCopy::AEnemyMageControllerCopy() {
	_movementCommandC = CreateDefaultSubobject<UAIMovementCommandComponent>(TEXT("AI Movement Command Component"));
	_perceptionC = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	_stateC = CreateDefaultSubobject<UStateComponent>(TEXT("State Component"));
}

void AEnemyMageControllerCopy::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	_stateC->PerformTickBehavior(deltaTime);

	_movementCommandC->GetMovementCommand()->OnMovementTick(this, deltaTime);

	_secondsSinceLastComfortTemperatureCheck += deltaTime;
	if (_secondsSinceLastComfortTemperatureCheck >= _secondsBetweenComfortTemperatureChecks) {
		_communicateTemperatureHarmfulness();
		_secondsSinceLastComfortTemperatureCheck = 0.0f;
	}
}

void AEnemyMageControllerCopy::BeginPlay() {
	Super::BeginPlay();

	_perceptionC->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyMageControllerCopy::_onActorSensed);

	if (const auto pawn = GetPawn(); IsValid(pawn)) {
		_temperatureDmgC = pawn->FindComponentByClass<UTemperatureDamageHandlerComponent>();
		if (!_temperatureDmgC.IsValid()) {
			UE_LOG(LogTemp, Error, TEXT("%s(): controlled pawn is missing Temperature Damage Handler Component"), *FString{__FUNCTION__});
		}

		_thermoC = pawn->FindComponentByClass<UThermodynamicComponent>();
		if (!_thermoC.IsValid()) {
			UE_LOG(LogTemp, Error, TEXT("%s(): controlled pawn is missing Thermodynamic Component"), *FString{__FUNCTION__});
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s(): Controlled Pawn is unset"), *FString{__FUNCTION__});
	}

	if(!IsValid(_behaviorTree)) {
		UE_LOG(LogTemp, Error, TEXT("%s(): missing Behavior Tree Class"), *FString{__FUNCTION__});
		return;
	}
	
	RunBehaviorTree(_behaviorTree);
}

void AEnemyMageControllerCopy::_onActorSensed(AActor* const actor, const FAIStimulus stimulus) {
	// Did we sense a player-controlled character?
	if (const auto character = Cast<ACharacter>(actor); IsValid(character) && character->IsPlayerControlled()) {
		const auto blackBoard = GetBlackboardComponent();
		blackBoard->SetValueAsBool(_canSeeKeyName, stimulus.WasSuccessfullySensed());
		blackBoard->SetValueAsObject(_targetKeyName, character);
	}
}

void AEnemyMageControllerCopy::_communicateTemperatureHarmfulness() {
	if (!_thermoC.IsValid() || !_temperatureDmgC.IsValid()) {
		return;
	}

	const auto currentTemperature = _thermoC->GetTemperature();
	const auto comfortUpperLimit = _temperatureDmgC->GetMaxComfortTemperature();
	const auto comfortLowerLimit = _temperatureDmgC->GetMinComfortTemperature();

	const bool isTemperatureHarmful = currentTemperature > comfortUpperLimit || currentTemperature < comfortLowerLimit;

	// Update the blackBoard on whether the controlled pawn is taking temperature damage or not
	const auto blackBoard = GetBlackboardComponent();
	blackBoard->SetValueAsBool(_temperatureHarmKeyName, isTemperatureHarmful);

	if (!isTemperatureHarmful) {
		return;
	}

	ETemperatureSearchBehaviorCopy searchBehavior = ETemperatureSearchBehaviorCopy::None;
	if (currentTemperature < comfortLowerLimit - _negativeComfortDeviationForBehaviorChange) {
		searchBehavior = ETemperatureSearchBehaviorCopy::HottestZone;
	} else if (currentTemperature < comfortUpperLimit + _positiveComfortDeviationForBehaviorChange) {
		searchBehavior = ETemperatureSearchBehaviorCopy::ComfortableZone;
	} else {
		searchBehavior = ETemperatureSearchBehaviorCopy::ColdestZone;
	}

	blackBoard->SetValueAsEnum(_temperatureSearchBehaviorKeyName, static_cast<uint8>(searchBehavior));
}
