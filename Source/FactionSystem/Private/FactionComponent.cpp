// Fill out your copyright notice in the Description page of Project Settings.

#include "FactionComponent.h"

ERelationship UFactionComponent::GetRelationshipWithFaction(EFaction const faction) const {
    if (auto const relationship = _relationships.Find(faction)) {
        return *relationship;
    }

    return ERelationship::Neutral;
}

void UFactionComponent::SetRelationshipWithFaction(EFaction const faction, ERelationship const relationship) {
    _relationships.FindOrAdd(faction) = relationship;
}

bool UFactionComponent::HasRelationshipWithFaction(EFaction const faction, ERelationship const queryRelationship) const {
    if (auto const relationshipWithFaction = _relationships.Find(faction)) {
        return *relationshipWithFaction == queryRelationship;
    }

    return queryRelationship == ERelationship::Neutral;
}

TSet<EFaction> UFactionComponent::GetFactionsWithRelationship(ERelationship const queryRelationship) const {
    auto const predicate = [queryRelationship](
                               TPair<EFaction, ERelationship> const& factionToRelationship) { return factionToRelationship.Value == queryRelationship; };

    TSet<EFaction> factionsWithRelationship;
    Algo::TransformIf(_relationships, factionsWithRelationship, predicate,
        [](TPair<EFaction, ERelationship> const& factionToRelationship) { return factionToRelationship.Key; });

    return factionsWithRelationship;
}
