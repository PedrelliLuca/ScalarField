// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTargetingConditions/FlammabilityTargetingCondition.h"

#include "MaterialsContainerComponent.h"
#include "SkillTargets/ActorSkillTarget.h"

bool UFlammabilityTargetingCondition::IsVerifiedForTarget(ISkillTarget* const skillTarget) const {
    check(skillTarget != nullptr);
    auto const actorSkillTarget = Cast<UActorSkillTarget>(skillTarget);
    if (!ensureMsgf(IsValid(actorSkillTarget), TEXT("This condition should only be applied to targets of type UActorSkillTarget!"))) {
        return false;
    }

    auto const materialsC = actorSkillTarget->GetActor()->FindComponentByClass<UMaterialsContainerComponent>();

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
