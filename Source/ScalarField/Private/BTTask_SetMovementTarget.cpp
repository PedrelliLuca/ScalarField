// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetMovementTarget.h"

#include "AIMovementCommandComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetMovementTarget::UBTTask_SetMovementTarget() {
    NodeName = "Set Movement Target";

    // Accept only actors and vectors
    BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetMovementTarget, BlackboardKey), AActor::StaticClass());
    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetMovementTarget, BlackboardKey));
}

EBTNodeResult::Type UBTTask_SetMovementTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) {
    const auto aiController = ownerComp.GetAIOwner();
    const auto aiMovementCommandC = aiController->FindComponentByClass<UAIMovementCommandComponent>();
    if (!IsValid(aiMovementCommandC)) {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI Controller does not have a Movement Command Component"), *FString{__FUNCTION__});
        return EBTNodeResult::Failed;
    }

    const auto blackboardC = ownerComp.GetBlackboardComponent();

    FVector targetLocation{};
    if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
        targetLocation = blackboardC->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

    } else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
        const auto keyValue = blackboardC->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
        const auto targetActor = Cast<AActor>(keyValue);
        check(IsValid(targetActor));
        targetLocation = targetActor->GetActorLocation();
    } else {
        checkNoEntry();
    }

    aiMovementCommandC->SetMovementParameters(_movementParameters);
    aiMovementCommandC->GetMovementCommand()->OnSetDestination(aiController, targetLocation);

    return EBTNodeResult::Succeeded;
}

FString UBTTask_SetMovementTarget::GetStaticDescription() const {
    FString keyDesc("invalid");
    if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() ||
        BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
        keyDesc = BlackboardKey.SelectedKeyName.ToString();
    }

    return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *keyDesc);
}

FName UBTTask_SetMovementTarget::GetNodeIconName() const {
    return FName("BTEditor.Graph.BTNode.Task.MoveTo.Icon");
}
