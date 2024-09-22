// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTests/EnvQueryTest_Relationship.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "FactionComponent.h"

UEnvQueryTest_Relationship::UEnvQueryTest_Relationship(FObjectInitializer const& objectInitializer)
    : Super(objectInitializer) {
    _querier = UEnvQueryContext_Querier::StaticClass();
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UEnvQueryTest_Relationship::RunTest(FEnvQueryInstance& queryInstance) const {
    auto const queryOwner = queryInstance.Owner.Get();
    if (queryOwner == nullptr) {
        return;
    }

    FloatValueMin.BindData(queryOwner, queryInstance.QueryID);
    float const filterMinThreshold = FloatValueMin.GetValue();

    FloatValueMax.BindData(queryOwner, queryInstance.QueryID);
    float const filterMaxThreshold = FloatValueMax.GetValue();

    TArray<AActor*> contextActors;
    if (!queryInstance.PrepareContext(_querier, contextActors)) {
        return;
    }

    // We expect a single querier
    check(contextActors.Num() == 1);

    auto const contextFactionC = contextActors.Last()->FindComponentByClass<UFactionComponent>();
    if (!IsValid(contextFactionC)) {
        UE_LOG(LogTemp, Error, TEXT("%s: the Context Actor doesn't have a UFactionComponent"), *FString{__FUNCTION__});
        return;
    }

    for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
        auto const otherActor = GetItemActor(queryInstance, it.GetIndex());

        auto const otherFactionC = otherActor->FindComponentByClass<UFactionComponent>();
        if (!IsValid(otherFactionC)) {
            continue;
        }

        auto const score = contextFactionC->GetRelationshipWithFaction(otherFactionC->GetFaction()) == _relationshipToSearchFor ? 1.0f : 0.0f;
        it.SetScore(TestPurpose, FilterType, score, filterMinThreshold, filterMaxThreshold);
    }
}

FText UEnvQueryTest_Relationship::GetDescriptionTitle() const {
    FString relationshipDescription;
    switch (_relationshipToSearchFor) {
        case ERelationship::Ally:
            relationshipDescription = TEXT("Ally");
            break;

        case ERelationship::Enemy:
            relationshipDescription = TEXT("Enemy");
            break;

        case ERelationship::Neutral:
            relationshipDescription = TEXT("Neutral");
            break;

        default:
            checkNoEntry();
    }

    // *Super::GetDescriptionTitle().ToString() returns "Relationship" because of how we named this class
    return FText::FromString(FString::Printf(TEXT("%s: %s"), *Super::GetDescriptionTitle().ToString(), *relationshipDescription));
}

FText UEnvQueryTest_Relationship::GetDescriptionDetails() const {
    return DescribeFloatTestParams();
}
