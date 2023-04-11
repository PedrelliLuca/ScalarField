// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTests/EnvQueryTest_Comfort.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicComponent.h"

UEnvQueryTest_Comfort::UEnvQueryTest_Comfort() {
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UEnvQueryTest_Comfort::RunTest(FEnvQueryInstance& queryInstance) const {
    const auto queryOwner = queryInstance.Owner.Get();
    if (queryOwner == nullptr) {
        return;
    }

    FloatValueMin.BindData(queryOwner, queryInstance.QueryID);
    const float filterMinThreshold = FloatValueMin.GetValue();

    FloatValueMax.BindData(queryOwner, queryInstance.QueryID);
    const float filterMaxThreshold = FloatValueMax.GetValue();

    switch (_testMode) {
        case EEnvTestComfort::Comfort:
            /* We look for actors that are comfortable with their temperature. */
            for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
                const auto otherActor = GetItemActor(queryInstance, it.GetIndex());

                const auto otherThermoDmgHandlerC = otherActor->FindComponentByClass<UTemperatureDamageHandlerComponent>();
                const auto otherThermoC = otherActor->FindComponentByClass<UThermodynamicComponent>();
                if (!IsValid(otherThermoDmgHandlerC) || !IsValid(otherThermoC)) {
                    continue;
                }

                const auto lowerComfortT = otherThermoDmgHandlerC->GetMinComfortTemperature();
                const auto upperComfortT = otherThermoDmgHandlerC->GetMaxComfortTemperature();
                const auto temperature = otherThermoC->GetTemperature();
                if (temperature < lowerComfortT || temperature > upperComfortT) {
                    continue;
                }

                const auto bestComfortT = (lowerComfortT + upperComfortT) * 0.5f;
                const auto delta = FMath::Abs(bestComfortT - temperature);

                /* In this case, with an Inverse Linear Scoring Equation the closer the other's temperature is to the mid of its
                 * comfort interval, the better it is. */
                it.SetScore(TestPurpose, FilterType, delta, filterMinThreshold, filterMaxThreshold);
            }
            break;
        case EEnvTestComfort::DiscomfortCold:
            /* We look for actors that are unfortable because their temperature is too low */
            for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
                const auto otherActor = GetItemActor(queryInstance, it.GetIndex());

                const auto otherThermoDmgHandlerC = otherActor->FindComponentByClass<UTemperatureDamageHandlerComponent>();
                const auto otherThermoC = otherActor->FindComponentByClass<UThermodynamicComponent>();
                if (!IsValid(otherThermoDmgHandlerC) || !IsValid(otherThermoC)) {
                    continue;
                }

                const auto lowerComfortT = otherThermoDmgHandlerC->GetMinComfortTemperature();
                const auto temperature = otherThermoC->GetTemperature();
                if (temperature > lowerComfortT) {
                    continue;
                }

                const auto delta = lowerComfortT - temperature;

                /* In this case, with an Inverse Linear Scoring Equation the closer the other's temperature is to its minimum comfort temperature and the better
                 * it is. */
                it.SetScore(TestPurpose, FilterType, delta, filterMinThreshold, filterMaxThreshold);
            }
            break;
        case EEnvTestComfort::DiscomfortHot:
            /* We look for actors that are unfortable because their temperature is too high */
            for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
                const auto otherActor = GetItemActor(queryInstance, it.GetIndex());

                const auto otherThermoDmgHandlerC = otherActor->FindComponentByClass<UTemperatureDamageHandlerComponent>();
                const auto otherThermoC = otherActor->FindComponentByClass<UThermodynamicComponent>();
                if (!IsValid(otherThermoDmgHandlerC) || !IsValid(otherThermoC)) {
                    continue;
                }

                const auto upperComfortT = otherThermoDmgHandlerC->GetMaxComfortTemperature();
                const auto temperature = otherThermoC->GetTemperature();
                if (temperature < upperComfortT) {
                    continue;
                }

                const auto delta = temperature - upperComfortT;

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
