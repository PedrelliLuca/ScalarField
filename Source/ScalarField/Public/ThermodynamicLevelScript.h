// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
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

private:
};
