// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Faction.h"
#include "Relationship.h"

#include "FactionComponent.generated.h"

UCLASS(ClassGroup = (Faction), meta = (BlueprintSpawnableComponent))
class FACTIONSYSTEM_API UFactionComponent : public UActorComponent {
    GENERATED_BODY()

public:
    EFaction GetFaction() const { return _faction; }
    void SetFaction(EFaction faction) { _faction = faction; }

    ERelationship GetRelationshipWithFaction(EFaction faction) const;
    void SetRelationshipWithFaction(EFaction faction, ERelationship relationship);

    /** \brief Returns whether the relationship with the given function is the expected one or not */
    bool HasRelationshipWithFaction(EFaction faction, ERelationship queryRelationship) const;

    bool IsAlliedWithFaction(EFaction const faction) const { return HasRelationshipWithFaction(faction, ERelationship::Ally); }
    bool IsEnemyWithFaction(EFaction const faction) const { return HasRelationshipWithFaction(faction, ERelationship::Enemy); }

    /** \brief Returns all factions whose relationship with this component is the input one. */
    TSet<EFaction> GetFactionsWithRelationship(ERelationship queryRelationship) const;

    TSet<EFaction> GetAlliedFactions() const { return GetFactionsWithRelationship(ERelationship::Ally); }
    TSet<EFaction> GetEnemyFactions() const { return GetFactionsWithRelationship(ERelationship::Enemy); }

private:
    UPROPERTY(EditDefaultsOnly, Category = "Faction Component | Faction")
    EFaction _faction = EFaction::Factionless;

    /**
     * \brief Stores the relationships of the owner actor with the various factions. The relationship with factions
     * that aren't in the map is ERelationship::Neutral
     */
    UPROPERTY(EditDefaultsOnly, Category = "Faction Component | Relationships")
    TMap<EFaction, ERelationship> _relationships;
};
