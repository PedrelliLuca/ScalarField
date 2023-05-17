// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargets/ActorSkillTarget.h"

FVector UActorSkillTarget::GetTargetLocation() const {
    check(_actor.IsValid());
    return _actor->GetActorLocation();
}

void UActorSkillTarget::SetActor(TObjectPtr<AActor> actor) {
    // Make sure that the caster is not valid before setting it.
    if (ensureMsgf(!_actor.IsValid(), TEXT("Target Actor can be set only once and has already been set."))) {
        check(IsValid(actor));
        _actor = MoveTemp(actor);
    }
}
