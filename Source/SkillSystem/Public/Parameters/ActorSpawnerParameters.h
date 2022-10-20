// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorSpawnerParameters.generated.h"

USTRUCT(Blueprintable)
struct FActorSpawnerParameters {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClass = nullptr;

	UPROPERTY(EditAnywhere)
	FTransform Transform = FTransform::Identity;
};
