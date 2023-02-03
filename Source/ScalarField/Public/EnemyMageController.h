// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

#include "EnemyMageController.generated.h"

/**
 * 
 */
UCLASS()
class SCALARFIELD_API AEnemyMageController : public AAIController {
	GENERATED_BODY()

public:
	AEnemyMageController();

protected:
	void BeginPlay() override;
	
private:
	UFUNCTION()
	void _onActorSensed(AActor* actor, FAIStimulus stimulus);

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Mage")
	FName _canSeeKeyName = FName{"CanSeeTarget"};

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Mage")
	FName _targetKeyName = FName{"Target"};

	UPROPERTY(VisibleAnywhere, Category = "Enemy AI")
	TObjectPtr<UAIPerceptionComponent> _perceptionC;
};
