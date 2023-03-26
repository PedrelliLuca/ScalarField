// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"

#include "HealingItem.generated.h"

UCLASS(Blueprintable)
class SCALARFIELD_API UHealingItem : public UInventoryItem {
    GENERATED_BODY()

public:
    UHealingItem();

    void Use(TWeakObjectPtr<AActor> actor) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing")
    double _healAmount;
};
