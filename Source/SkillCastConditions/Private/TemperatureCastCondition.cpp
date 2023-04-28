// Fill out your copyright notice in the Description page of Project Settings.

#include "TemperatureCastCondition.h"

bool UTemperatureCastCondition::IsVerified() const {
    return false;
}

void UTemperatureCastCondition::SetConditionSubject(TObjectPtr<AActor> subject) {
    check(IsValid(subject));
    _conditionSubject = MoveTemp(subject);
}
