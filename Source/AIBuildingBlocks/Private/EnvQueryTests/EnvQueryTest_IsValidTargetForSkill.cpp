// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTests/EnvQueryTest_IsValidTargetForSkill.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "NewSkillsContainerComponent.h"
#include "SkillsContainerInspector.h"
#include "SkillTargets/ActorSkillTarget.h"

UEnvQueryTest_IsValidTargetForSkill::UEnvQueryTest_IsValidTargetForSkill(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer) {
    _querier = UEnvQueryContext_Querier::StaticClass();
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UEnvQueryTest_IsValidTargetForSkill::RunTest(FEnvQueryInstance& queryInstance) const {
    const auto queryOwner = queryInstance.Owner.Get();
    if (queryOwner == nullptr) {
        return;
    }

    FloatValueMin.BindData(queryOwner, queryInstance.QueryID);
    const float filterMinThreshold = FloatValueMin.GetValue();

    FloatValueMax.BindData(queryOwner, queryInstance.QueryID);
    const float filterMaxThreshold = FloatValueMax.GetValue();

    TArray<AActor*> contextActors;
    if (!queryInstance.PrepareContext(_querier, contextActors)) {
        return;
    }

    // We expect a single querier
    check(contextActors.Num() == 1);

    const auto skillsContainerC = contextActors.Last()->FindComponentByClass<UNewSkillsContainerComponent>();
    if (!ensureMsgf(IsValid(skillsContainerC), TEXT("The Context Actor doesn't have a UNewSkillsContainerComponent"))) {
        return;
    }

    const auto containerInspector = FSkillsContainerInspector{skillsContainerC};
    const auto optionalIdx = containerInspector.GetIndexOfSkill(_skill);
    if (!ensureMsgf(optionalIdx.IsSet(), TEXT("The Context Actor doesn't have the selected skill"))) {
        return;
    }

    const auto optionalSkillInspector = containerInspector.GetSkillPropertiesByIndex(*optionalIdx);
    check(optionalSkillInspector.IsSet());

    const auto& targetingConditions = optionalSkillInspector->GetTargetingConditions();
    for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
        const auto otherActor = GetItemActor(queryInstance, it.GetIndex());

        bool otherVerifiesAllConditions = true;

        ISkillTarget* target = NewObject<UActorSkillTarget>(const_cast<UEnvQueryTest_IsValidTargetForSkill*>(this), UActorSkillTarget::StaticClass());
        target->Init(FSkillTargetPacket{otherActor});
        
        for (const auto& targetingCondition : targetingConditions) {
            if (!targetingCondition->IsVerifiedForTarget(target)) {
                otherVerifiesAllConditions = false;
                break;
            }
        }

        const auto score = otherVerifiesAllConditions ? 1.0f : 0.0f;
        it.SetScore(TestPurpose, FilterType, score, filterMinThreshold, filterMaxThreshold);
    }
}

FText UEnvQueryTest_IsValidTargetForSkill::GetDescriptionTitle() const {
    // *Super::GetDescriptionTitle().ToString() returns "IsValidTargetForSkill" because of how we named this class
    return FText::FromString(FString::Printf(TEXT("%s"), *Super::GetDescriptionTitle().ToString()));
}

FText UEnvQueryTest_IsValidTargetForSkill::GetDescriptionDetails() const {
    return DescribeFloatTestParams();
}
