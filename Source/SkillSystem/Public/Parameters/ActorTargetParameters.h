// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorTargetParameters.generated.h"

USTRUCT(Blueprintable)
struct FActorTargetParameters {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Actor Components")
	TArray<TSubclassOf<UActorComponent>> RequiredComponents{};

	UPROPERTY(EditAnywhere, Category = "Range")
	double MaxDistance = 0.;

	TWeakObjectPtr<AActor> Target;
};
