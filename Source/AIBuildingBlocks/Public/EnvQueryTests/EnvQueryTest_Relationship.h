// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnvironmentQuery/EnvQueryTest.h"
#include "Relationship.h"

#include "EnvQueryTest_Relationship.generated.h"

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UEnvQueryTest_Relationship : public UEnvQueryTest {
    GENERATED_BODY()

public:
    UEnvQueryTest_Relationship(const FObjectInitializer& objectInitializer);

private:
    void RunTest(FEnvQueryInstance& QueryInstance) const override;

    FText GetDescriptionTitle() const override;
    FText GetDescriptionDetails() const override;

    /** testing mode */
    UPROPERTY(EditDefaultsOnly, Category = "Relationship")
    ERelationship _relationshipToSearchFor;

    /** context */
    UPROPERTY(EditDefaultsOnly, Category = "Relationship")
    TSubclassOf<UEnvQueryContext> _querier;
};
