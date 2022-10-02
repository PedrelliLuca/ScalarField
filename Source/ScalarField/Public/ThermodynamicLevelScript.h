// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/TriggerBox.h"
#include "EnvironmentGridWorldSubsystem.h"

#include "ThermodynamicLevelScript.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Thermodynamics), NotPlaceable, Blueprintable)
class SCALARFIELD_API AThermodynamicLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()

public:

protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Environment Grid")
	double _gridStep = 100.;

private:
	UEnvironmentGridWorldSubsystem::FGridSpawnAttributes _buildGridSpawnAttributes(TObjectPtr<ATriggerBox> gridTriggerBox);

};
