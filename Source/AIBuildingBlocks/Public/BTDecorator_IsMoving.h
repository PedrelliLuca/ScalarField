// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsMoving.generated.h"

UENUM()
enum class EMovementCheckKind : uint8
{
    IsMoving UMETA(DisplayName = "Is Moving"),
    IsNotMoving UMETA(DisplayName = "Is not Moving")
};

/**
 *
 */
UCLASS()
class AIBUILDINGBLOCKS_API UBTDecorator_IsMoving : public UBTDecorator {
    GENERATED_BODY()

protected:
    bool CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const override;

private:
    FString GetStaticDescription() const override;

    UPROPERTY(EditAnywhere)
    EMovementCheckKind _movementCheck = EMovementCheckKind::IsMoving;
};
