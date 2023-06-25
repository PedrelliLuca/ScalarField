// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CastSpell.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/RunEQSComponent.h"
#include "ManaComponent.h"
#include "NewSkillsContainerComponent.h"
#include "NewStateComponent.h"
#include "SkillPropertiesInspector.h"
#include "SkillTargets/ActorSkillTarget.h"
#include "SkillTargets/CasterPlaneLocationSkillTarget.h"
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
    if (!ensureMsgf(IsValid(stateC), TEXT("AI-controlled Pawn does not have a State Component"))) {
        return EBTNodeResult::Failed;
    }

    const auto skillsContainerC = TObjectPtr<UNewSkillsContainerComponent>{pawn->FindComponentByClass<UNewSkillsContainerComponent>()};
    if (!ensureMsgf(IsValid(skillsContainerC), TEXT("AI-controlled Pawn does not have a Skills Container Component"))) {
        return EBTNodeResult::Failed;
    }

    const auto skillsContainerInsp = FSkillsContainerInspector{skillsContainerC};
    const auto optionalSkillIdx = FSkillsContainerInspector{skillsContainerC}.GetIndexOfSkill(_newSkillToCast);
    if (!ensureMsgf(optionalSkillIdx.IsSet(), TEXT("AI-controlled Pawn does not have the selected Skill"))) {
        return EBTNodeResult::Failed;
    }

    const auto optionalSkillPropertiesInsp = skillsContainerInsp.GetSkillPropertiesByIndex(*optionalSkillIdx);
    check(optionalSkillPropertiesInsp.IsSet());

    if (_needsManaAvailabilityToCast && !_newIsManaAvailableForSkill(pawn, optionalSkillPropertiesInsp->GetTotalManaCost())) {
        // The designer requested for all mana to be immediately available but we don't have it yet.
        return EBTNodeResult::Failed;
    }

    const auto optionalSkillCastResult = stateC->TryCastSkillAtIndex(*optionalSkillIdx);
    if (!optionalSkillIdx.IsSet()) {
        // The AI-controlled pawn is in a state where they can't cast any skill.
        return EBTNodeResult::Failed;
    }

    if (!optionalSkillCastResult->IsFailure()) {
        return EBTNodeResult::Succeeded;
    }

    if (optionalSkillCastResult->GetCastResult() != ESkillCastResult::Fail_MissingTarget) {
        return EBTNodeResult::Failed;
    }

    const auto eqsComponent = ownerComp.GetAIOwner()->FindComponentByClass<URunEQSComponent>();
    if (!ensureMsgf(IsValid(eqsComponent), TEXT("AI-controlled Pawn trying to cast a targeting spell without a RunEQSComponent"))) {
        return EBTNodeResult::Failed;
    }

    auto queryItemsIterator = eqsComponent->GetQueryItemsIterator();
    for (; !queryItemsIterator.IsDone(); ++queryItemsIterator) {
        auto itemRawData = *queryItemsIterator;

        FSkillTargetPacket targetPacket{};

        const auto targetKind = optionalSkillPropertiesInsp->GetTargetKind();

        // TODO: add another else if here managing UGroundLocationSkillTarget when you'll have skills using this kind of target, e.g. "Spawn Tree" or something

        // UBTService_RunEQS::OnQueryFinished() calls UEnvQueryItemType_ActorBase::StoreInBlackboard(). This causes...
        if (targetKind == UCasterPlaneLocationSkillTarget::StaticClass()) {
            // ... Super::StoreInBlackboard() to be called => UEnvQueryItemType_Point::GetItemLocation() into UEnvQueryItemType_Point::GetValue() is called.
            const auto navLocation = *reinterpret_cast<FNavLocation*>(const_cast<uint8*>(itemRawData));

            const auto location = navLocation.Location;
            const auto casterPlane = FPlane{0.0f, 0.0f, 1.0f, pawn->GetActorLocation().Z};
            const auto casterPlaneLocation = FMath::LinePlaneIntersection(
                location + FVector::UpVector * PROJ_LINE_HALF_LENGTH, location - FVector::UpVector * PROJ_LINE_HALF_LENGTH, casterPlane);

            targetPacket.TargetCasterPlaneLocation = casterPlaneLocation;
        } else if (targetKind == UActorSkillTarget::StaticClass()) {
            // ... EnvQueryItemType_Actor::GetActor() into UEnvQueryItemType_Actor::GetValue() to be called.
            auto weakObjPtr = *reinterpret_cast<FWeakObjectPtr*>(const_cast<uint8*>(itemRawData));
            const auto rawActor = reinterpret_cast<AActor*>(weakObjPtr.Get());
            if (!IsValid(rawActor)) {
                continue;
            }
            targetPacket.TargetActor = rawActor;
        } else {
            checkNoEntry();
        }

        auto stateResponse = stateC->TrySetSkillTarget(MoveTemp(targetPacket));
        /* The response is unset when:
         * 1. The state we're in does not allow targeting. However, if a state allows casting it should also allow targeting, and we established above that the
         * current state allows casting
         * 2. There is no waiting skill in the skillsContainerC. However, the conditions above ensured that the skill we're trying to cast is waiting for
         * targets, so there must be a waiting skill.
         *
         * The 2 listed points imply that something is going horribly wrong if the stateResponse is unset here.
         */
        check(stateResponse.IsSet());

        if (stateResponse->IsType<FSkillCastResult>()) {
            // We managed to provide all targets to the skill. Is the cast successful though?
            return stateResponse->Get<FSkillCastResult>().IsFailure() ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
        }
    }

    // If we get here it means that we cycled through all the EQS Query Items and didn't manage to provide enough targets => forget the casting of the skill.
    stateC->TryAbort();
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
