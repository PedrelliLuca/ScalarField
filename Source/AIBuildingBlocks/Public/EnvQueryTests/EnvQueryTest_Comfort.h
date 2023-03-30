// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnvironmentQuery/EnvQueryTest.h"

#include "EnvQueryTest_Comfort.generated.h"

UENUM()
namespace EEnvTestComfort {
enum Type
{
    Comfort UMETA(DisplayName = "Comfort"),
    DiscomfortCold UMETA(DisplayName = "Discomfort: Too Cold"),
    DiscomfortHot UMETA(DisplayName = "Discomfort: Too Hot")
};
}

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UEnvQueryTest_Comfort : public UEnvQueryTest {
    GENERATED_BODY()

public:
    UEnvQueryTest_Comfort();

private:
    void RunTest(FEnvQueryInstance& QueryInstance) const override;

    FText GetDescriptionTitle() const override;
    FText GetDescriptionDetails() const override;

    /** testing mode */
    UPROPERTY(EditDefaultsOnly, Category = Temperature)
    TEnumAsByte<EEnvTestComfort::Type> _testMode;
};
