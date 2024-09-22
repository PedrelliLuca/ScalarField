// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TargetsBasedMovement.h"

#include "AIController.h"
#include "AIMovementCommandComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UBTTask_TargetsBasedMovement::UBTTask_TargetsBasedMovement() {
    NodeName = "Targets-Based Movement";

    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_TargetsBasedMovement, BlackboardKey));
}

EBTNodeResult::Type UBTTask_TargetsBasedMovement::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    auto const aiController = ownerComp.GetAIOwner();
    auto const aiMovementCommandC = aiController->FindComponentByClass<UAIMovementCommandComponent>();
    if (!IsValid(aiMovementCommandC)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI Controller does not have a Movement Command Component"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    auto const runEQSC = aiController->FindComponentByClass<URunEQSComponent>();
    if (!ensureMsgf(IsValid(runEQSC), TEXT("AI-controlled Pawn trying to perform target-based movement without a RunEQSComponent"))) {
        return EBTNodeResult::Failed;
    }

    // TODO: edit this once EQ with vector item types will be supported
    [[maybe_unused]] auto const itemsType = runEQSC->GetQueryItemsType();
    if (!ensureMsgf(itemsType.IsSet() && *itemsType == UEnvQueryItemType_Actor::StaticClass(),
            TEXT("AI-controlled Pawn trying to perform target-based movement without a RunEQSComponent"))) {
        return EBTNodeResult::Failed;
    }

    FVector targetLocation{};
    switch (_movementStrategy) {
        case ETargetsBasedMovementStrategy::TBMS_MoveToFirstTarget:
            targetLocation = _getLocation_MoveToFirstTargetStrategy(runEQSC->GetQueryItemsIterator());
            break;
        case ETargetsBasedMovementStrategy::TBMS_MoveToMidPoint:
            targetLocation = _getLocation_MidPointStrategy(runEQSC->GetQueryItemsIterator());
            break;
        default:
            checkNoEntry();
    }

    aiMovementCommandC->SetMovementParameters(_movementParameters);
    aiMovementCommandC->SetDestination(targetLocation); // TODO: use state component here

    auto const blackboardC = ownerComp.GetBlackboardComponent();
    blackboardC->SetValueAsVector(BlackboardKey.SelectedKeyName, targetLocation);

    return EBTNodeResult::Succeeded;
}

FString UBTTask_TargetsBasedMovement::GetStaticDescription() const {
    // TODO:
    return Super::GetStaticDescription();
}

#if WITH_EDITOR
FName UBTTask_TargetsBasedMovement::GetNodeIconName() const {
    return FName("BTEditor.Graph.BTNode.Task.MoveTo.Icon");
}
#endif

FVector UBTTask_TargetsBasedMovement::_getLocation_MoveToFirstTargetStrategy(FQueryItemsIterator&& itemsIter) const {
    check(!itemsIter.IsDone());
    auto itemRawData = *itemsIter;
    auto weakObjPtr = *reinterpret_cast<FWeakObjectPtr*>(const_cast<uint8*>(itemRawData));
    auto const rawActor = reinterpret_cast<AActor*>(weakObjPtr.Get());
    return rawActor->GetActorLocation();
}

FVector UBTTask_TargetsBasedMovement::_getLocation_MidPointStrategy(FQueryItemsIterator&& itemsIter) const {
    FVector location = FVector::ZeroVector;
    int32 i = 0;
    /* NOTE: TARGETS != ITEMS! You could have your EQ returning more items than the needed ones (i.e. the targets). This component is just a draft, it should be
     * reworked to take only the needed items into account */
    for (; !itemsIter.IsDone(); ++itemsIter) {
        auto itemRawData = *itemsIter;
        auto weakObjPtr = *reinterpret_cast<FWeakObjectPtr*>(const_cast<uint8*>(itemRawData));
        auto const rawActor = reinterpret_cast<AActor*>(weakObjPtr.Get());
        if (IsValid(rawActor)) { // Necessary, the raw actor might be dead by the time you get here
            location += rawActor->GetActorLocation();
            ++i;
        }
    }
    return location / i;
}
