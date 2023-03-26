// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CancelMovement.h"

#include "AIController.h"
#include "AIMovementCommandComponent.h"

UBTTask_CancelMovement::UBTTask_CancelMovement() {
    NodeName = "Cancel Movement";
}

EBTNodeResult::Type UBTTask_CancelMovement::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    const auto aiController = ownerComp.GetAIOwner();
    const auto aiMovementCommandC = aiController->FindComponentByClass<UAIMovementCommandComponent>();
    if (!IsValid(aiMovementCommandC)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI Controller does not have a Movement Command Component"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    aiMovementCommandC->GetMovementCommand()->OnStopMovement(aiController);

    return EBTNodeResult::Succeeded;
}
