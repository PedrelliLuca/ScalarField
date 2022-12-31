// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ItemAddResult.generated.h"

UENUM()
enum class EItemAddResult : uint8 {
	IAR_NoItemsAdded UMETA(DisplayName = "No items added"),
	IAR_SomeItemsAdded UMETA(DisplayName = "Some items added"),
	IAR_AllItemsAdded UMETA(DisplayName = "All items added")
};

// Represents the result of adding an item to the inventory
USTRUCT(BlueprintType)
struct FItemAddResult {
	GENERATED_BODY()

public:
	FItemAddResult() {}
	FItemAddResult(int32 itemQuantity) : ItemQuantity(itemQuantity), ItemQuantityActuallyGiven(0) {};
	FItemAddResult(int32 itemQuantity, int32 itemQuantityActuallyGiven) : ItemQuantity(itemQuantity), ItemQuantityActuallyGiven(itemQuantityActuallyGiven) {};

	// Functions that make building instances of this struct easier
	static FItemAddResult AddedNone(int32 itemQuantity, const FText& errorText);
	static FItemAddResult AddedSome(int32 itemQuantity, int32 itemQuantityActuallyGiven, const FText& errorText);
	static FItemAddResult AddedAll(int32 itemQuantity);

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ItemQuantity = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ItemQuantityActuallyGiven = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult Result = EItemAddResult::IAR_NoItemsAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ErrorText {};
};

#define LOCTEXT_NAMESPACE "ItemAddResult"

FORCEINLINE FItemAddResult FItemAddResult::AddedNone(int32 itemQuantity, const FText& errorText) {
	FItemAddResult addResult{itemQuantity};
	addResult.Result = EItemAddResult::IAR_NoItemsAdded;
	addResult.ErrorText = errorText;
	
	return addResult;
}

FORCEINLINE FItemAddResult FItemAddResult::AddedSome(int32 itemQuantity, int32 itemQuantityActuallyGiven, const FText& errorText) {
	FItemAddResult addResult{itemQuantity, itemQuantityActuallyGiven};
	addResult.Result = EItemAddResult::IAR_SomeItemsAdded;
	addResult.ErrorText = errorText;
	
	return addResult;
}

FORCEINLINE FItemAddResult FItemAddResult::AddedAll(int32 itemQuantity) {
	FItemAddResult addResult{itemQuantity};
	addResult.Result = EItemAddResult::IAR_AllItemsAdded;
	
	return addResult;
}

#undef LOCTEXT_NAMESPACE