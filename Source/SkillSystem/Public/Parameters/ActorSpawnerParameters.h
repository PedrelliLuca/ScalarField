// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorSpawnerParameters.generated.h"

USTRUCT(Blueprintable)
struct FActorSpawnerParameters {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Actor")
	TSubclassOf<AActor> ActorClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Actor")
	FTransform Transform = FTransform::Identity;
};
