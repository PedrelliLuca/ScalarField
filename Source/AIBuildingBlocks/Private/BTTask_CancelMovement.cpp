// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CancelMovement.h"

#include "AIController.h"
#include "AIMovementCommandComponent.h"

UBTTask_CancelMovement::UBTTask_CancelMovement() {
    NodeName = "Cancel Movement";
}

EBTNodeResult::Type UBTTask_CancelMovement::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    auto const aiController = ownerComp.GetAIOwner();
    auto const aiMovementCommandC = aiController->FindComponentByClass<UAIMovementCommandComponent>();
    if (!IsValid(aiMovementCommandC)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI Controller does not have a Movement Command Component"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    aiMovementCommandC->StopMovement();

    return EBTNodeResult::Succeeded;
}
