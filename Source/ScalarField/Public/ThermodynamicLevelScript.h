// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/TriggerBox.h"
#include "EnvironmentGridWorldSubsystem.h"
#include "AirMolecule.h"

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

	UPROPERTY(EditAnywhere, Category = "Environment Grid", meta = (ClampMin = "100"))
	double _gridStep = 100.;

	UPROPERTY(EditAnywhere, Category = "Air", meta = (ClampMin = "0"))
	double _airTemperature = 298.15;

	UPROPERTY(EditAnywhere, Category = "Air", meta = (ClampMin = "1"))
	double _airHeatCapacity = 700.;

	UPROPERTY(EditAnywhere, Category = "Air", meta = (ClampMin = "2"))
	int32 _moleculesPerCellSide = 3;

	UPROPERTY(EditAnywhere, NoClear, Category = "Air")
	TSubclassOf<AAirMolecule> _moleculeClass;

private:
	UEnvironmentGridWorldSubsystem::FGridSpawnAttributes _buildGridSpawnAttributes(TObjectPtr<ATriggerBox> gridTriggerBox);

	/**
	 * \brief For each cell of the environmental grid, spawns a 2D lattice of thermodynamic actors meant to behave as air, i.e.
	 * to fill space between other thermodynamic actors in the level while transmitting heat.
	 */
	void _generateAir(double cellSide);
};
