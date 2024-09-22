// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingConditions/HasComponentsTargetingCondition.h"

#include "Algo/AllOf.h"
#include "SkillTargets/ActorSkillTarget.h"

bool UHasComponentsTargetingCondition::IsVerifiedForTarget(ISkillTarget* const skillTarget) const {
    check(skillTarget != nullptr);
    check(!_requiredComponents.IsEmpty());

    auto const actorSkillTarget = Cast<UActorSkillTarget>(skillTarget);
    if (!ensureMsgf(IsValid(actorSkillTarget), TEXT("This condition should only be applied to targets of type UActorSkillTarget!"))) {
        return false;
    }

    return Algo::AllOf(_requiredComponents,
        [targetActor = actorSkillTarget->GetActor()](auto const& requiredComponent) { return IsValid(targetActor->FindComponentByClass(requiredComponent)); });
}
