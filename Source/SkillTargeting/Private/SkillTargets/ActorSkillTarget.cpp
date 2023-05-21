// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargets/ActorSkillTarget.h"

void UActorSkillTarget::Init(const FSkillTargetPacket& targetPacket) {
    auto actor = targetPacket.TargetActor;
    // Make sure that the actor is not valid before setting it.
    if (ensureMsgf(!_actor.IsValid(), TEXT("Target Actor can be set only once and has already been set."))) {
        check(actor.IsValid());
        _actor = MoveTemp(actor);
    }
}

FVector UActorSkillTarget::GetTargetLocation() const {
    check(_actor.IsValid());
    return _actor->GetActorLocation();
}
