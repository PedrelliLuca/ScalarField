// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "CollisionsCollectionComponent.generated.h"

/**
 * \brief TODO
 */
UCLASS(ClassGroup = "Collision", meta = (BlueprintSpawnableComponent))
class COLLISIONSCOLLECTION_API UCollisionsCollectionComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UCollisionsCollectionComponent();
};
