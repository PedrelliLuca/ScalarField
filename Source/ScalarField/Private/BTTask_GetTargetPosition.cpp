// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GetTargetPosition.h"

#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_GetTargetPosition::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    const auto blackboard = ownerComp.GetBlackboardComponent();
    check(IsValid(blackboard));

    const auto target = Cast<AActor>(blackboard->GetValueAsObject(TargetSelector.SelectedKeyName));
    check(IsValid(target));

    const auto navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!IsValid(navSys)) {
        UE_LOG(LogTemp, Error, TEXT("%s: invalid Navigation System!"), *FString{__FUNCTION__});
    }

    const auto targetLoc = target->GetActorLocation();
    if (!RandomizeInRadius) {
        blackboard->SetValueAsVector(PositionSelector.SelectedKeyName, targetLoc);
        return EBTNodeResult::Succeeded;
    }

    FNavLocation randomPoint{targetLoc};
    if (navSys->GetRandomPointInNavigableRadius(targetLoc, Radius, randomPoint)) {
        blackboard->SetValueAsVector(PositionSelector.SelectedKeyName, randomPoint.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
