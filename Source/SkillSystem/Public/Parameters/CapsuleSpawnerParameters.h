// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CapsuleSpawnerParameters.generated.h"

USTRUCT(Blueprintable)
struct FCapsuleSpawnerParameters {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double HalfHeight = 0.;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	double Radius = 0.;

	UPROPERTY(EditAnywhere)
	FTransform Transform = FTransform::Identity;
};