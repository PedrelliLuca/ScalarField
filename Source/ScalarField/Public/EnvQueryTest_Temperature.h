// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_Temperature.generated.h"

UENUM()
namespace EEnvTestTemperature {
enum Type
{
    Absolute,
    Relative
};
}

/**
 *
 */
UCLASS()
class SCALARFIELD_API UEnvQueryTest_Temperature : public UEnvQueryTest {
    GENERATED_BODY()

public:
    UEnvQueryTest_Temperature(const FObjectInitializer& objectInitializer);

private:
    void RunTest(FEnvQueryInstance& QueryInstance) const override;

    FText GetDescriptionTitle() const override;
    FText GetDescriptionDetails() const override;

    /** testing mode */
    UPROPERTY(EditDefaultsOnly, Category = Temperature)
    TEnumAsByte<EEnvTestTemperature::Type> _testMode;

    /** context */
    UPROPERTY(EditDefaultsOnly, Category = Temperature)
    TSubclassOf<UEnvQueryContext> _querier;
};
