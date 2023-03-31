// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CastSpellOnTarget.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SkillsContainerComponent.h"

UBTTask_CastSpellOnTarget::UBTTask_CastSpellOnTarget() {
    NodeName = TEXT("Cast Spell on Target");

    // Only actor targets are allowed by the FSM at the time being.
    BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CastSpellOnTarget, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_CastSpellOnTarget::_determineCastSuccessWithTarget(
    const UBehaviorTreeComponent& ownerComp, const TObjectPtr<UStateComponent>& stateC, const TObjectPtr<UAbstractSkill>& skill) {
    if (BlackboardKey.IsNone()) {
        UE_LOG(LogTemp, Error, TEXT("%s(): Skill requires target, but BlackboardKey is None"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    // Target selected, start execution!
    const auto isExecutionStarted = stateC->PerformSkillExecutionBehavior(skill);
    if (!isExecutionStarted) {
        // The excution start can fail for many valid reason (e.g. in case the skill is on cooldown), don't throw errors from here!
        return EBTNodeResult::Failed;
    }

    // Execution has started, let's see if the targeting operation is successful.
    const auto keyValue = ownerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
    const auto targetActor = Cast<AActor>(keyValue);
    const auto isTargetingSuccessful = stateC->PerformTargetingBehavior(targetActor);

    if (isTargetingSuccessful) {
        return EBTNodeResult::Succeeded;
    }

    // Getting here means that either:
    // A) The BT logic is passing an invalid target (which is an AI design mistake)
    // B) The target was fine per se, but the skills requires more than one of them, so the targeting phase must continue.
    //
    // At the moment, there isn't a single skill I made that requires multiple targets, so we should always get here because of A). A) is an AI design mistake:
    // we must signal it, then abort the skill execution and revert to idle state.
    // In the future, when B) will happen, we'll have to distinguish it from A) somehow. B) is not a design error: this node will have to handle multiple
    // BlackboardKeys and call PerformTargetingBehavior() for each one of them.

    UE_LOG(LogTemp, Error, TEXT("%s(): Skill targeting failed, invalid target in input! Check the skill requirements and the BT."), *FString{__FUNCTION__});
    stateC->PerformAbortBehavior();
    return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_CastSpellOnTarget::_determineCastSuccessWithoutTarget(
    const TObjectPtr<UStateComponent>& stateC, const TObjectPtr<UAbstractSkill>& skill) {
    return stateC->PerformSkillExecutionBehavior(skill) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_CastSpellOnTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
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

    const auto stateC = aiController->FindComponentByClass<UStateComponent>();
    if (!IsValid(stateC)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AIController does not have a State Component"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    const auto skill = skillsContainerC->FindSkillByClass(_skillToCast);
    if (!IsValid(skill)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI-Controlled Pawn does not have the selected Skill"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    if (!skill->RequiresTarget()) {
        UE_LOG(LogTemp, Error, TEXT("%s(): Selected skill does not require a target. Employ \"Cast Spell\" node."), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    // Target selected, start execution!
    const auto isExecutionStarted = stateC->PerformSkillExecutionBehavior(skill);
    if (!isExecutionStarted) {
        // The excution start can fail for many valid reason (e.g. in case the skill is on cooldown), don't throw errors from here!
        return EBTNodeResult::Failed;
    }

    // Execution has started, let's see if the targeting operation is successful.
    const auto keyValue = ownerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
    const auto targetActor = Cast<AActor>(keyValue);
    const auto isTargetingSuccessful = stateC->PerformTargetingBehavior(targetActor);

    if (isTargetingSuccessful) {
        return EBTNodeResult::Succeeded;
    }

    // The targeting operation returned false. This means that either:
    // A) The BT logic is passing an invalid target (which is an AI design mistake)
    // B) The target was fine per se, but the skills requires more than one of them, so the targeting phase must continue.
    //
    // At the moment, there isn't a single skill I made that requires multiple targets, so we should always get here because of A). A) is an AI design mistake:
    // we must signal it, then abort the skill execution and revert to idle state.
    // In the future, when B) will happen, we'll have to distinguish it from A) somehow (enum?). B) is not a design error: this node will have to handle
    // multiple BlackboardKeys and call PerformTargetingBehavior() for each one of them.

    UE_LOG(LogTemp, Error, TEXT("%s(): Skill targeting failed, invalid target in input! Check the skill requirements and the BT."), *FString{__FUNCTION__});
    stateC->PerformAbortBehavior();
    return EBTNodeResult::Failed;
}

FString UBTTask_CastSpellOnTarget::GetStaticDescription() const {
    FString skillDesc{"invalid"};
    if (IsValid(_skillToCast)) {
        skillDesc = _skillToCast->GetClass()->GetName();
    }

    FString targetDesc{"invalid"};
    if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
        targetDesc = BlackboardKey.SelectedKeyName.ToString();
    }

    return FString::Printf(TEXT("%s: %s on %s"), *Super::GetStaticDescription(), *skillDesc, *targetDesc);
}
