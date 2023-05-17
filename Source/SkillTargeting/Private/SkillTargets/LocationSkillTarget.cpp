// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargets/LocationSkillTarget.h"

FVector ULocationSkillTarget::GetTargetLocation() const {
    check(_hasBeenSet);
    return _location;
}

void ULocationSkillTarget::SetLocation(const FVector& location) {
    // Make sure that the caster is not valid before setting it.
    if (ensureMsgf(!_hasBeenSet, TEXT("Condition Location can be set only once and has already been set."))) {
        _location = location;
        _hasBeenSet = true;
    }
}
