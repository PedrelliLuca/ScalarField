// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargets/ActorSkillTarget.h"

void UActorSkillTarget::Init(const FSkillTargetPacket& targetPacket) {
    auto actor = targetPacket.TargetActor;
    // Make sure that the actor is not valid before setting it.
    if (ensureMsgf(!_actor.IsValid(), TEXT("Target Actor can be set only once and has already been set."))) {
        _actor = MoveTemp(actor);
    }
}

bool UActorSkillTarget::IsValidTarget() const {
    return _actor.IsValid();
}

FVector UActorSkillTarget::GetTargetLocation() const {
    check(IsValidTarget());
    return _actor->GetActorLocation();
}

TObjectPtr<AActor> UActorSkillTarget::GetActor() const {
    return _actor.Get();
}
