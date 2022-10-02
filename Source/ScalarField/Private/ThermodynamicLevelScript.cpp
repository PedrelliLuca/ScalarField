// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicLevelScript.h"

#include "EnvironmentGridWorldSubsystem.h"

void AThermodynamicLevelScript::BeginPlay() {
	Super::BeginPlay();

	const auto environmentGridSubsys = GetWorld()->GetSubsystem<UEnvironmentGridWorldSubsystem>();
	environmentGridSubsys->SpawnGrid();
}