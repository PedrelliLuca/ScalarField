// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargets/CasterPlaneLocationSkillTarget.h"

void UCasterPlaneLocationSkillTarget::Init(FSkillTargetPacket const& targetPacket) {
    check(targetPacket.TargetCasterPlaneLocation.IsSet());
    auto location = *targetPacket.TargetCasterPlaneLocation;
    // Make sure that the location is not valid before setting it.
    if (ensureMsgf(!_hasBeenSet, TEXT("Condition Location can be set only once and has already been set."))) {
        _location = MoveTemp(location);
        _hasBeenSet = true;
    }
}

FVector UCasterPlaneLocationSkillTarget::GetTargetLocation() const {
    check(_hasBeenSet);
    return _location;
}
