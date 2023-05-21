// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingConditions/HasComponentsTargetingCondition.h"

#include "Algo/AllOf.h"
#include "SkillTargets/ActorSkillTarget.h"

bool UHasComponentsTargetingCondition::IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const {
    check(!_requiredComponents.IsEmpty());

    const auto actorSkillTarget = Cast<UActorSkillTarget>(skillTarget.GetObject());
    if (!ensureMsgf(IsValid(actorSkillTarget), TEXT("This condition should only be applied to targets of type UActorSkillTarget!"))) {
        return false;
    }

    return Algo::AllOf(_requiredComponents,
        [targetActor = actorSkillTarget->GetActor()](const auto& requiredComponent) { return IsValid(targetActor->FindComponentByClass(requiredComponent)); });
}
