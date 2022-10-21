// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorSpawnerParameters.h"

#include "FollowerActorSpawnerParameters.generated.h"

USTRUCT(Blueprintable)
struct FFollowerActorSpawnerParameters : public FActorSpawnerParameters {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	double Damping = 0.;
};