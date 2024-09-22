// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTests/EnvQueryTest_IsValidTargetForSkill.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "SkillPropertiesInspector.h"
#include "SkillTargets/ActorSkillTarget.h"
#include "SkillsContainerComponent.h"
#include "SkillsContainerInspector.h"

UEnvQueryTest_IsValidTargetForSkill::UEnvQueryTest_IsValidTargetForSkill(FObjectInitializer const& objectInitializer)
    : Super(objectInitializer) {
    _querier = UEnvQueryContext_Querier::StaticClass();
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UEnvQueryTest_IsValidTargetForSkill::RunTest(FEnvQueryInstance& queryInstance) const {
    auto const queryOwner = queryInstance.Owner.Get();
    if (queryOwner == nullptr) {
        return;
    }

    FloatValueMin.BindData(queryOwner, queryInstance.QueryID);
    float const filterMinThreshold = FloatValueMin.GetValue();

    FloatValueMax.BindData(queryOwner, queryInstance.QueryID);
    float const filterMaxThreshold = FloatValueMax.GetValue();

    TArray<AActor*> contextActors;
    if (!queryInstance.PrepareContext(_querier, contextActors)) {
        return;
    }

    // We expect a single querier
    check(contextActors.Num() == 1);

    auto const skillsContainerC = contextActors.Last()->FindComponentByClass<USkillsContainerComponent>();
    if (!ensureMsgf(IsValid(skillsContainerC), TEXT("The Context Actor doesn't have a USkillsContainerComponent"))) {
        return;
    }

    auto const containerInspector = FSkillsContainerInspector{skillsContainerC};
    auto const optionalIdx = containerInspector.GetIndexOfSkill(_skill);
    if (!ensureMsgf(optionalIdx.IsSet(), TEXT("The Context Actor doesn't have the selected skill"))) {
        return;
    }

    auto const optionalSkillInspector = containerInspector.GetSkillPropertiesByIndex(*optionalIdx);
    check(optionalSkillInspector.IsSet());

    auto const& targetingConditions = optionalSkillInspector->GetTargetingConditions();
    for (FEnvQueryInstance::ItemIterator it(this, queryInstance); it; ++it) {
        auto const otherActor = GetItemActor(queryInstance, it.GetIndex());

        bool otherVerifiesAllConditions = true;

        ISkillTarget* target = NewObject<UActorSkillTarget>(const_cast<UEnvQueryTest_IsValidTargetForSkill*>(this), UActorSkillTarget::StaticClass());
        target->Init(FSkillTargetPacket{otherActor});

        for (auto const& targetingCondition : targetingConditions) {
            if (!targetingCondition->IsVerifiedForTarget(target)) {
                otherVerifiesAllConditions = false;
                break;
            }
        }

        auto const score = otherVerifiesAllConditions ? 1.0f : 0.0f;
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
