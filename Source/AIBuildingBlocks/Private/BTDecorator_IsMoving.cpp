// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsMoving.h"

#include "AIController.h"
#include "AIMovementCommandComponent.h"

bool UBTDecorator_IsMoving::CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* const nodeMemory) const {
    const auto aiController = ownerComp.GetAIOwner();
    const auto aiMovementC = aiController->FindComponentByClass<UAIMovementCommandComponent>();
    if (!ensureMsgf(IsValid(aiMovementC), TEXT("AI-controlled Pawn does not have a valid AI Movement Command Component"))) {
        return EBTNodeResult::Failed;
    }

    switch (_movementCheck) {
        case EMovementCheckKind::IsMoving:
            return aiMovementC->IsMoving();
        case EMovementCheckKind::IsNotMoving:
            return !aiMovementC->IsMoving();
        default:
            checkNoEntry();
    }

    return false;
}

FString UBTDecorator_IsMoving::GetStaticDescription() const {
    auto descr = FString{"Is "};
    switch (_movementCheck) {
        case EMovementCheckKind::IsMoving:
            break;
        case EMovementCheckKind::IsNotMoving:
            descr += FString{"not "};
            break;
        default:
            checkNoEntry();
    }
    descr += "Moving";
    return descr;
}
