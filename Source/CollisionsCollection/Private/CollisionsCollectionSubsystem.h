// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "CollisionsCollectionSubsystem.generated.h"

class UCollisionsCollectionComponent;
class UPrimitiveComponent;

/**
 * \brief TODO
 */
UCLASS()
class UCollisionsCollectionSubsystem : public UWorldSubsystem {
    GENERATED_BODY()

public:
    void AddCollection(TWeakObjectPtr<UCollisionsCollectionComponent> collection);
    void RemoveCollection(TWeakObjectPtr<UCollisionsCollectionComponent> collection);

    TWeakObjectPtr<UCollisionsCollectionComponent> GetCollectionByElement(const UPrimitiveComponent* element) const;

private:
    TMap<TWeakObjectPtr<UPrimitiveComponent>, TWeakObjectPtr<UCollisionsCollectionComponent>> _elementToCollection;
};
