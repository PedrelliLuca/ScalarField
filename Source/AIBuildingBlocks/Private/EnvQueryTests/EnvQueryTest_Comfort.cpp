// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTests/EnvQueryTest_Comfort.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicsInteractorComponent.h"

UEnvQueryTest_Comfort::UEnvQueryTest_Comfort() {
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UEnvQueryTest_Comfort::RunTest(FEnvQueryInstance& queryInstance) const {
    auto const queryOwner = queryInstance.Owner.Get();
    if (queryOwner == nullptr) {
        return;
    }

    FloatValueMin.BindData(queryOwner, queryInstance.QueryID);
    float const filterMinThreshold = FloatValueMin.GetValue();

    FloatValueMax.BindData(queryOwner, queryInstance.QueryID);
    float const filterMaxThreshold = FloatValueMax.GetValue();

    switch (_testMode) {
        case EEnvTestComfort::Comfort:
            /* We look for actors that are comfortable with their temperature. */
            for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
                auto const otherActor = GetItemActor(queryInstance, it.GetIndex());

                auto const otherThermoDmgHandlerC = otherActor->FindComponentByClass<UTemperatureDamageHandlerComponent>();
                auto const otherThermoC = otherActor->FindComponentByClass<UThermodynamicsInteractorComponent>();
                if (!IsValid(otherThermoDmgHandlerC) || !IsValid(otherThermoC)) {
                    continue;
                }

                auto const lowerComfortT = otherThermoDmgHandlerC->GetMinComfortTemperature();
                auto const upperComfortT = otherThermoDmgHandlerC->GetMaxComfortTemperature();
                auto const temperature = otherThermoC->GetTemperature();
                if (temperature < lowerComfortT || temperature > upperComfortT) {
                    continue;
                }

                auto const bestComfortT = (lowerComfortT + upperComfortT) * 0.5f;
                auto const delta = FMath::Abs(bestComfortT - temperature);

                /* In this case, with an Inverse Linear Scoring Equation the closer the other's temperature is to the mid of its
                 * comfort interval, the better it is. */
                it.SetScore(TestPurpose, FilterType, delta, filterMinThreshold, filterMaxThreshold);
            }
            break;
        case EEnvTestComfort::DiscomfortCold:
            /* We look for actors that are unfortable because their temperature is too low */
            for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
                auto const otherActor = GetItemActor(queryInstance, it.GetIndex());

                auto const otherThermoDmgHandlerC = otherActor->FindComponentByClass<UTemperatureDamageHandlerComponent>();
                auto const otherThermoC = otherActor->FindComponentByClass<UThermodynamicsInteractorComponent>();
                if (!IsValid(otherThermoDmgHandlerC) || !IsValid(otherThermoC)) {
                    continue;
                }

                auto const lowerComfortT = otherThermoDmgHandlerC->GetMinComfortTemperature();
                auto const temperature = otherThermoC->GetTemperature();
                if (temperature > lowerComfortT) {
                    continue;
                }

                auto const delta = lowerComfortT - temperature;

                /* In this case, with an Inverse Linear Scoring Equation the closer the other's temperature is to its minimum comfort temperature and the better
                 * it is. */
                it.SetScore(TestPurpose, FilterType, delta, filterMinThreshold, filterMaxThreshold);
            }
            break;
        case EEnvTestComfort::DiscomfortHot:
            /* We look for actors that are unfortable because their temperature is too high */
            for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
                auto const otherActor = GetItemActor(queryInstance, it.GetIndex());

                auto const otherThermoDmgHandlerC = otherActor->FindComponentByClass<UTemperatureDamageHandlerComponent>();
                auto const otherThermoC = otherActor->FindComponentByClass<UThermodynamicsInteractorComponent>();
                if (!IsValid(otherThermoDmgHandlerC) || !IsValid(otherThermoC)) {
                    continue;
                }

                auto const upperComfortT = otherThermoDmgHandlerC->GetMaxComfortTemperature();
                auto const temperature = otherThermoC->GetTemperature();
                if (temperature < upperComfortT) {
                    continue;
                }

                auto const delta = temperature - upperComfortT;

                /* In this case, with an Inverse Linear Scoring Equation the closer the other's temperature is to its maximumx comfort temperature and the
                 * better it is. */
                it.SetScore(TestPurpose, FilterType, delta, filterMinThreshold, filterMaxThreshold);
            }
            break;
        default:
            checkNoEntry();
    };
}

FText UEnvQueryTest_Comfort::GetDescriptionTitle() const {
    FString description;
    switch (_testMode) {
        case EEnvTestComfort::Comfort:
            description = TEXT("Actors Comfortable with their Temperature");
            break;
        case EEnvTestComfort::DiscomfortCold:
            description = TEXT("Actors Uncomfortable: too Cold for them");
            break;
        case EEnvTestComfort::DiscomfortHot:
            description = TEXT("Actors Uncomfortable: too Hot for them");
            break;
        default:
            checkNoEntry();
    }

    // *Super::GetDescriptionTitle().ToString() returns "Relationship" because of how we named this class
    return FText::FromString(FString::Printf(TEXT("%s"), *description));
}

FText UEnvQueryTest_Comfort::GetDescriptionDetails() const {
    return DescribeFloatTestParams();
}
