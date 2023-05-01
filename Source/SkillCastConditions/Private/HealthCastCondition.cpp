// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthCastCondition.h"

bool UHealthCastCondition::IsVerified() const {
    check(_subjectHealthC.IsValid());

    bool isVerified = false;
    switch (_healthComparison) {
        case EHealthComparisonStyle::HCS_LessThan:
            isVerified = _subjectHealthC->GetCurrentHealth() < _castConditionHealth;
            break;
        case EHealthComparisonStyle::HCS_GreaterThan:
            isVerified = _subjectHealthC->GetCurrentHealth() > _castConditionHealth;
            break;
        default:
            checkNoEntry();
    };

    return isVerified;
}

void UHealthCastCondition::SetConditionSubject(TObjectPtr<AActor> subject) {
    // Make sure that the subject is not valid before setting it, this setter is only for initialization.
    if (ensureMsgf(!_conditionSubject.IsValid(), TEXT("Condition Subject can be set only once and has already been set."))) {
        check(IsValid(subject));
        _conditionSubject = MoveTemp(subject);

        _subjectHealthC = _conditionSubject->FindComponentByClass<UHealthComponent>();
        check(_subjectHealthC.IsValid());
    }
}
