// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTest_Temperature.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicComponent.h"

UEnvQueryTest_Temperature::UEnvQueryTest_Temperature(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer) {
    _querier = UEnvQueryContext_Querier::StaticClass();
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UEnvQueryTest_Temperature::RunTest(FEnvQueryInstance& queryInstance) const {
    UObject* const queryOwner = queryInstance.Owner.Get();
    if (queryOwner == nullptr) {
        return;
    }

    FloatValueMin.BindData(queryOwner, queryInstance.QueryID);
    const float filterMinThreshold = FloatValueMin.GetValue();

    FloatValueMax.BindData(queryOwner, queryInstance.QueryID);
    const float filterMaxThreshold = FloatValueMax.GetValue();

    TArray<AActor*> contextActors;
    if (!queryInstance.PrepareContext(_querier, contextActors)) {
        return;
    }

    // We expect a single querier
    check(contextActors.Num() == 1);

    const auto contextTempDmgC = contextActors[0]->FindComponentByClass<UTemperatureDamageHandlerComponent>();
    if (!IsValid(contextTempDmgC)) {
        UE_LOG(LogTemp, Error, TEXT("%s: the Context Actor doesn't have a UTemperatureDamageHandlerComponent"), *FString{__FUNCTION__});
        return;
    }

    switch (_testMode) {
        case EEnvTestTemperature::Absolute:
            for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
                const auto otherActor = GetItemActor(queryInstance, it.GetIndex());

                const auto otherThermoC = otherActor->FindComponentByClass<UThermodynamicComponent>();
                if (!IsValid(otherThermoC)) {
                    UE_LOG(LogTemp, Error, TEXT("%s: iterating over Actor without UThermodynamicComponent"), *FString{__FUNCTION__});
                    continue;
                }

                /* In this case, with a Linear Scoring Equation the higher the temperature the better the score.
                With an Inverse Linear Scoring Equation, the lower the temperature the higher the score*/
                it.SetScore(TestPurpose, FilterType, otherThermoC->GetTemperature(), filterMinThreshold, filterMaxThreshold);
            }
            break;
        case EEnvTestTemperature::Relative:
            for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
                const auto otherActor = GetItemActor(queryInstance, it.GetIndex());

                const auto otherThermoC = otherActor->FindComponentByClass<UThermodynamicComponent>();
                if (!IsValid(otherThermoC)) {
                    UE_LOG(LogTemp, Error, TEXT("%s: iterating over Actor without UThermodynamicComponent"), *FString{__FUNCTION__});
                    continue;
                }

                const auto midComfort = (contextTempDmgC->GetMinComfortTemperature() + contextTempDmgC->GetMaxComfortTemperature()) * 0.5;
                const auto deltaTemperature = FMath::Abs(midComfort - otherThermoC->GetTemperature());

                /* In this case, with an Inverse Linear Scoring Equation the closer the other's temperature is to the mid of the
                 * comfort interval, the better it is.*/
                it.SetScore(TestPurpose, FilterType, deltaTemperature, filterMinThreshold, filterMaxThreshold);
            }
            break;
        default:
            checkNoEntry();
            return;
    }
}

FText UEnvQueryTest_Temperature::GetDescriptionTitle() const {
    FString modeDescription;
    switch (_testMode) {
        case EEnvTestTemperature::Absolute:
            modeDescription = TEXT("Absolute");
            break;

        case EEnvTestTemperature::Relative:
            modeDescription = TEXT("Relative");
            break;

        default:
            checkNoEntry();
    }

    // *Super::GetDescriptionTitle().ToString() returns "Temperature" because of how we named this class
    return FText::FromString(FString::Printf(TEXT("%s %s"), *modeDescription, *Super::GetDescriptionTitle().ToString()));
}

FText UEnvQueryTest_Temperature::GetDescriptionDetails() const {
    return DescribeFloatTestParams();
}
