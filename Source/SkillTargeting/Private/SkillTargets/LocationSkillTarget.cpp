// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargets/LocationSkillTarget.h"

void ULocationSkillTarget::Init(const FSkillTargetPacket& targetPacket) {
    auto location = targetPacket.TargetLocation;
    // Make sure that the location is not valid before setting it.
    if (ensureMsgf(!_hasBeenSet, TEXT("Condition Location can be set only once and has already been set."))) {
        _location = MoveTemp(location);
        _hasBeenSet = true;
    }
}

FVector ULocationSkillTarget::GetTargetLocation() const {
    check(_hasBeenSet);
    return _location;
}
