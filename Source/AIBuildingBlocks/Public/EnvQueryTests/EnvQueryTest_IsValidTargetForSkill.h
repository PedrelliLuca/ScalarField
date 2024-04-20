// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnvironmentQuery/EnvQueryTest.h"
#include "AbstractSkill.h"

#include "EnvQueryTest_IsValidTargetForSkill.generated.h"

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UEnvQueryTest_IsValidTargetForSkill : public UEnvQueryTest {
    GENERATED_BODY()

public:
    UEnvQueryTest_IsValidTargetForSkill(const FObjectInitializer& objectInitializer);

private:
    void RunTest(FEnvQueryInstance& QueryInstance) const override;

    FText GetDescriptionTitle() const override;
    FText GetDescriptionDetails() const override;

    /** testing mode */
    UPROPERTY(EditDefaultsOnly, Category = "Relationship")
    TSubclassOf<UAbstractSkill> _skill;

    /** context */
    UPROPERTY(EditDefaultsOnly, Category = "Relationship")
    TSubclassOf<UEnvQueryContext> _querier;
};
