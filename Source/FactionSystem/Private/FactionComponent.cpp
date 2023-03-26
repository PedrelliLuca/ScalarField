// Fill out your copyright notice in the Description page of Project Settings.

#include "FactionComponent.h"

ERelationship UFactionComponent::GetRelationshipWithFaction(const EFaction faction) const {
    if (const auto relationship = _relationships.Find(faction)) {
        return *relationship;
    }

    return ERelationship::Neutral;
}

void UFactionComponent::SetRelationshipWithFaction(const EFaction faction, const ERelationship relationship) {
    _relationships.FindOrAdd(faction) = relationship;
}

bool UFactionComponent::HasRelationshipWithFaction(const EFaction faction, const ERelationship queryRelationship) const {
    if (const auto relationshipWithFaction = _relationships.Find(faction)) {
        return *relationshipWithFaction == queryRelationship;
    }

    return queryRelationship == ERelationship::Neutral;
}

TSet<EFaction> UFactionComponent::GetFactionsWithRelationship(const ERelationship queryRelationship) const {
    const auto predicate = [queryRelationship](
                               const TPair<EFaction, ERelationship>& factionToRelationship) { return factionToRelationship.Value == queryRelationship; };

    TSet<EFaction> factionsWithRelationship;
    Algo::TransformIf(_relationships, factionsWithRelationship, predicate,
        [](const TPair<EFaction, ERelationship>& factionToRelationship) { return factionToRelationship.Key; });

    return factionsWithRelationship;
}
