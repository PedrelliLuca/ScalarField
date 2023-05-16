// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingConditions/IgnitableTargetingCondition.h"

#include "MaterialsContainerComponent.h"
#include "SkillTargets/ActorSkillTarget.h"

bool UIgnitableTargetingCondition::IsVerifiedForTarget(const TScriptInterface<ISkillTarget> skillTarget) const {
    const auto actorSkillTarget = Cast<UActorSkillTarget>(skillTarget.GetObject());
    if (!IsValid(actorSkillTarget)) {
        return false;
    }

    const auto materialsC = actorSkillTarget->GetActor()->FindComponentByClass<UMaterialsContainerComponent>();

    bool isVerified = false;

    switch (_flammability) {
        case EFlammability::F_Ignitable:
            isVerified = IsValid(materialsC) && materialsC->HasMaterial(EMaterial::M_Carbon);
            break;
        case EFlammability::F_NotIgnitable:
            isVerified = !IsValid(materialsC) || !materialsC->HasMaterial(EMaterial::M_Carbon);
            break;
        default:
            checkNoEntry();
    }

    return isVerified;
}
