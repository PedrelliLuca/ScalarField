// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionsCollectionSubsystem.h"

#include "CollisionsCollectionComponent.h"

void UCollisionsCollectionSubsystem::AddCollection(TWeakObjectPtr<UCollisionsCollectionComponent> collection) {
    for (auto element : collection->_collectionElements) {
        _elementToCollection.Emplace(element, collection);
    }
}

void UCollisionsCollectionSubsystem::RemovePendingKillCollection(UCollisionsCollectionComponent* collection) {
    check(!IsValid(collection));

    for (auto element : collection->_collectionElements) {
        [[maybe_unused]] int32 const nRemoved = _elementToCollection.Remove(element);
        check(nRemoved == 1);
    }
}

TWeakObjectPtr<UCollisionsCollectionComponent> UCollisionsCollectionSubsystem::GetCollectionByElement(UPrimitiveComponent const* element) const {
    return _elementToCollection.FindChecked(element);
}
