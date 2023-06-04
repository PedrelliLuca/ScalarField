// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "Components/RunEQSComponent.h"
#include "CoreMinimal.h"
#include "Parameters/MovementParameters.h"

#include "BTTask_TargetsBasedMovement.generated.h"

// Might be refactored to use an actual strategy pattern in the future.
UENUM(BlueprintType)
enum class ETargetsBasedMovementStrategy : uint8
{
    TBMS_None UMETA(DisplayName = "None"),
    TBMS_MoveToFirstTarget UMETA(DisplayName = "Move to First Target"),
    TBMS_MoveToMidPoint UMETA(DisplayName = "Move to Mid Point"),
};

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UBTTask_TargetsBasedMovement : public UBTTaskNode {
    GENERATED_BODY()

public:
    UBTTask_TargetsBasedMovement();

private:
    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

    FString GetStaticDescription() const override;

#if WITH_EDITOR
    FName GetNodeIconName() const override;
#endif // WITH_EDITOR

    FVector _getLocation_MoveToFirstTargetStrategy(FQueryItemsIterator&& itemsIter) const;
    FVector _getLocation_MidPointStrategy(FQueryItemsIterator&& itemsIter) const;

    UPROPERTY(EditAnywhere)
    FMovementParameters _movementParameters;

    UPROPERTY(EditAnywhere)
    ETargetsBasedMovementStrategy _movementStrategy = ETargetsBasedMovementStrategy::TBMS_None;
};
