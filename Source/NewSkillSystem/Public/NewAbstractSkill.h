// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NewAbstractSkill.generated.h"

UENUM()
enum class ESkillExecutionResult : uint8
{
    IAR_NoItemsAdded UMETA(DisplayName = "No items added"),
    IAR_SomeItemsAdded UMETA(DisplayName = "Some items added"),
    IAR_AllItemsAdded UMETA(DisplayName = "All items added")
};

// Represents the result of requesting the execution of a skill
struct FSkillExecutionResult {
public:
    FSkillExecutionResult() {}
    FSkillExecutionResult(int32 itemQuantity)
        : ItemQuantity(itemQuantity)
        , ItemQuantityActuallyGiven(0){};
    FSkillExecutionResult(int32 itemQuantity, int32 itemQuantityActuallyGiven)
        : ItemQuantity(itemQuantity)
        , ItemQuantityActuallyGiven(itemQuantityActuallyGiven){};

    // Functions that make building instances of this struct easier
    static FSkillExecutionResult ExecutionFailed(int32 itemQuantity, const FText& errorText);
    static FSkillExecutionResult ExecutionNeedsTargets(int32 itemQuantity, int32 itemQuantityActuallyGiven, const FText& errorText);
    static FSkillExecutionResult ExecutionStarted(int32 itemQuantity);

    UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
    EItemAddResult Result = EItemAddResult::IAR_NoItemsAdded;

    UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
    FText ErrorText{};
};

/**
 *
 */
UCLASS(NotBlueprintable, Abstract)
class NEWSKILLSYSTEM_API UNewAbstractSkill : public UObject {
    GENERATED_BODY()

public:
    virtual FSkillExecutionResult TryCast() = 0;
};
