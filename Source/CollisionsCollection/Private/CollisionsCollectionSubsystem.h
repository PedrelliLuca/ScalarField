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

    // This function is meant to be called during UCollisionsCollectionComponent::BeginDestroy(), when the collection is marked for pending kill.
    // TWeakObjectPtrs of pending kill objects crash when operator->() (aka FWeakObjectPtr::Get()) is called, which is way a raw pointer is used insted.
    void RemovePendingKillCollection(UCollisionsCollectionComponent* collection);

    TWeakObjectPtr<UCollisionsCollectionComponent> GetCollectionByElement(UPrimitiveComponent const* element) const;

private:
    TMap<TWeakObjectPtr<UPrimitiveComponent>, TWeakObjectPtr<UCollisionsCollectionComponent>> _elementToCollection;
};
