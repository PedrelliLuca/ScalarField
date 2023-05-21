// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingConditions/DerivesFromTargetingCondition.h"

#include "SkillTargets/ActorSkillTarget.h"

bool UDerivesFromTargetingCondition::IsVerifiedForTarget(TScriptInterface<ISkillTarget> skillTarget) const {
    const auto actorSkillTarget = Cast<UActorSkillTarget>(skillTarget.GetObject());
    if (!ensureMsgf(IsValid(actorSkillTarget), TEXT("This condition should only be applied to targets of type UActorSkillTarget!"))) {
        return false;
    }

    return actorSkillTarget->GetActor()->IsA(_classToDeriveFrom);
}