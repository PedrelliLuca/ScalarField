// Fill out your copyright notice in the Description page of Project Settings.

#include "HealingItem.h"

#include "HealthComponent.h"

#define LOCTEXT_NAMESPACE "HealingItem"

UHealingItem::UHealingItem() {
    _healAmount = 20.0;
    _actionText = LOCTEXT("Action", "Consume");
}

void UHealingItem::Use(TWeakObjectPtr<AActor> actor) {
    Super::Use(actor);

    TWeakObjectPtr<UHealthComponent> const healthC = actor->FindComponentByClass<UHealthComponent>();
    if (healthC.IsValid()) {
        healthC->Heal(_healAmount);
    }
}

#undef LOCTEXT_NAMESPACE
