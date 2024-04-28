// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionsCollectionSubsystem.h"

#include "CollisionsCollectionComponent.h"

void UCollisionsCollectionSubsystem::AddCollection(TWeakObjectPtr<UCollisionsCollectionComponent> collection) {
    for (auto element : collection->_collectionElements) {
        _elementToCollection.Emplace(element, collection);
    }
}

void UCollisionsCollectionSubsystem::RemoveCollection(TWeakObjectPtr<UCollisionsCollectionComponent> collection) {
    for (auto element : collection->_collectionElements) {
        [[maybe_unused]] const int32 nRemoved = _elementToCollection.Remove(element);
        check(nRemoved == 1);
    }
}

TWeakObjectPtr<UCollisionsCollectionComponent> UCollisionsCollectionSubsystem::GetCollectionByElement(const UPrimitiveComponent* element) const {
    return _elementToCollection.FindChecked(element);
}
