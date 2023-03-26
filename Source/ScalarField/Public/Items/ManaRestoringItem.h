// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"

#include "ManaRestoringItem.generated.h"

UCLASS(Blueprintable)
class SCALARFIELD_API UManaRestoringItem : public UInventoryItem {
    GENERATED_BODY()

public:
    UManaRestoringItem();

    void Use(TWeakObjectPtr<AActor> actor) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing")
    double _manaRestoringAmount;
};
