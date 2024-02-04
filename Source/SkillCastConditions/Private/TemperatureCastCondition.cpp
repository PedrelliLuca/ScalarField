// Fill out your copyright notice in the Description page of Project Settings.

#include "TemperatureCastCondition.h"

#include "ThermodynamicsInteractorComponent.h"

bool UTemperatureCastCondition::IsVerified() const {
    check(_subjectThermoC.IsValid());

    bool isVerified = false;
    switch (_temperatureComparison) {
        case ETemperatureComparisonStyle::TCS_Equality:
            isVerified = FMath::IsNearlyEqual(_subjectThermoC->GetTemperature(), _castConditionTemperature);
            break;
        case ETemperatureComparisonStyle::TCS_LessThan:
            isVerified = _subjectThermoC->GetTemperature() < _castConditionTemperature;
            break;
        case ETemperatureComparisonStyle::TCS_GreaterThan:
            isVerified = _subjectThermoC->GetTemperature() > _castConditionTemperature;
            break;
        default:
            checkNoEntry();
    };

    return isVerified;
}

void UTemperatureCastCondition::SetSkillCaster(TObjectPtr<AActor> skillCaster) {
    // Make sure that the subject is not valid before setting it.
    if (ensureMsgf(!_conditionSubject.IsValid(), TEXT("Condition Subject can be set only once and has already been set."))) {
        check(IsValid(skillCaster));
        _conditionSubject = MoveTemp(skillCaster);

        _subjectThermoC = _conditionSubject->FindComponentByClass<UThermodynamicsInteractorComponent>();
        check(_subjectThermoC.IsValid());
    }
}
