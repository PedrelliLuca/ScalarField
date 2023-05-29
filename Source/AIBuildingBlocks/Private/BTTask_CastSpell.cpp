// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CastSpell.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ManaComponent.h"
#include "NewSkillsContainerComponent.h"
#include "NewStateComponent.h"
#include "SkillPropertiesInspector.h"
#include "SkillsContainerComponent.h"
#include "SkillsContainerInspector.h"
#include "StateComponent.h"

UBTTask_CastSpell::UBTTask_CastSpell() {
    NodeName = TEXT("Cast Spell");
}

EBTNodeResult::Type UBTTask_CastSpell::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    return _bNewSkillSystem ? _executeTaskNew(ownerComp, nodeMemory) : _executeTaskLegacy(ownerComp, nodeMemory);
}

EBTNodeResult::Type UBTTask_CastSpell::_executeTaskLegacy(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    if (!IsValid(_skillToCast)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): Skill to cast hasn't been selected"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    const auto aiController = ownerComp.GetAIOwner();
    const auto skillsContainerC = aiController->GetPawn()->FindComponentByClass<USkillsContainerComponent>();
    if (!IsValid(skillsContainerC)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI-Controlled Pawn does not have a Skills Container Component"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    auto skill = skillsContainerC->FindSkillByClass(_skillToCast);
    if (!IsValid(skill)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI-Controlled Pawn does not have the selected Skill"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    const auto stateC = aiController->FindComponentByClass<UStateComponent>();
    if (!IsValid(stateC)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AIController does not have a State Component"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    if (skill->RequiresTarget()) {
        UE_LOG(LogTemp, Error, TEXT("%s(): Selected skill does not require a target. Employ \"Cast Spell on Target\" node."), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    if (!_needsManaAvailabilityToCast || _isManaAvailableForSkill(aiController, skill)) {
        // The spell execution can fail, for example:
        // - In case the skill is on cooldown
        // - In case the skill is already being executed
        // - In case the current state does not allow the execution of skills
        return stateC->PerformSkillExecutionBehavior(MoveTemp(skill)) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
    }

    return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_CastSpell::_executeTaskNew(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    const auto pawn = ownerComp.GetAIOwner()->GetPawn();
    check(IsValid(pawn));

    const auto stateC = pawn->FindComponentByClass<UNewStateComponent>();
    if (!ensureAlwaysMsgf(IsValid(stateC), TEXT("AI-controlled Pawn does not have a State Component"))) {
        return EBTNodeResult::Failed;
    }

    const auto skillsContainerC = TObjectPtr<UNewSkillsContainerComponent>{pawn->FindComponentByClass<UNewSkillsContainerComponent>()};
    if (!ensureAlwaysMsgf(IsValid(skillsContainerC), TEXT("AI-controlled Pawn does not have a Skills Container Component"))) {
        return EBTNodeResult::Failed;
    }

    const auto skillsContainerInsp = FSkillsContainerInspector{skillsContainerC};
    const auto optionalSkillIdx = FSkillsContainerInspector{skillsContainerC}.GetIndexOfSkill(_newSkillToCast);
    if (!ensureAlwaysMsgf(optionalSkillIdx.IsSet(), TEXT("AI-controlled Pawn does not have the selected Skill"))) {
        return EBTNodeResult::Failed;
    }

    const auto optionalSkillPropertiesInsp = skillsContainerInsp.GetSkillPropertiesByIndex(*optionalSkillIdx);
    check(optionalSkillPropertiesInsp.IsSet());

    if (!_needsManaAvailabilityToCast || _newIsManaAvailableForSkill(pawn, optionalSkillPropertiesInsp->GetTotalManaCost())) {
        // TODO:
        // return stateC->TryCastSkillAtIndex(*optionalSkillIdx);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

FString UBTTask_CastSpell::GetStaticDescription() const {
    FString skillDesc{"invalid"};
    if (IsValid(_skillToCast)) {
        skillDesc = _skillToCast->GetClass()->GetName();
    }

    return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *skillDesc);
}

bool UBTTask_CastSpell::_isManaAvailableForSkill(const TObjectPtr<AAIController>& aiController, const TObjectPtr<UAbstractSkill>& skill) const {
    if (const auto pawn = aiController->GetPawn(); IsValid(pawn)) {
        if (const auto manaC = pawn->FindComponentByClass<UManaComponent>(); IsValid(manaC)) {
            return manaC->GetCurrentMana() >= skill->GetCastManaCost() + skill->GetChannelingManaCost();
        }
    }

    return false;
}

bool UBTTask_CastSpell::_newIsManaAvailableForSkill(const TObjectPtr<APawn>& pawn, float skillManaCost) const {
    if (const auto manaC = pawn->FindComponentByClass<UManaComponent>(); IsValid(manaC)) {
        return manaC->GetCurrentMana() >= skillManaCost;
    }

    // Pawns without a mana component can cast for free!
    return true;
}
