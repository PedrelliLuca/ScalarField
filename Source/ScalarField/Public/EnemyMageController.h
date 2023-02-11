// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicComponent.h"

#include "EnemyMageController.generated.h"

UENUM(BlueprintType)
enum class ETemperatureSearchBehavior : uint8 {
	None,
	ComfortableZone,
	HottestZone,
	ColdestZone
};

/**
 * 
 */
UCLASS()
class SCALARFIELD_API AEnemyMageController : public AAIController {
	GENERATED_BODY()

public:
	AEnemyMageController();

	void Tick(float deltaTime) override;

protected:
	void BeginPlay() override;
	
private:
	UFUNCTION()
	void _onActorSensed(AActor* actor, FAIStimulus stimulus);

	void _communicateTemperatureHarmfulness();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Mage Blackboard")
	FName _canSeeKeyName = FName{"CanSeeTarget"};

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Mage Blackboard")
	FName _targetKeyName = FName{"Target"};

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Mage Blackboard")
	FName _temperatureHarmKeyName = FName{ "IsTemperatureHarmful" };

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Mage Blackboard")
	FName _temperatureSearchBehaviorKeyName = FName{ "TemperatureSearchBehavior" };

	UPROPERTY(EditDefaultsOnly, Category = "Temperature Search Behavior")
	float _positiveComfortDeviationForBehaviorChange = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Temperature Search Behavior")
	float _negativeComfortDeviationForBehaviorChange = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "Mage AI")
	TObjectPtr<UAIPerceptionComponent> _perceptionC;

	UPROPERTY(EditDefaultsOnly,  Category = "Mage AI")
	TObjectPtr<UBehaviorTree> _behaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Mage AI")
	float _secondsBetweenComfortTemperatureChecks = 1.0f;

	float _secondsSinceLastComfortTemperatureCheck = 0.0f;

	TWeakObjectPtr<UTemperatureDamageHandlerComponent> _temperatureDmgC;
	TWeakObjectPtr<UThermodynamicComponent> _thermoC;
};