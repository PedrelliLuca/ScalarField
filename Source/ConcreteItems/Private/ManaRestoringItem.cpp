// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaRestoringItem.h"

#include "ManaComponent.h"

#define LOCTEXT_NAMESPACE "ManaRestoringItem"

UManaRestoringItem::UManaRestoringItem() {
    _manaRestoringAmount = 10.0;
    _actionText = LOCTEXT("Action", "Consume");
}

void UManaRestoringItem::Use(TWeakObjectPtr<AActor> actor) {
    Super::Use(actor);

    TWeakObjectPtr<UManaComponent> const manaC = actor->FindComponentByClass<UManaComponent>();
    if (manaC.IsValid()) {
        manaC->RestoreMana(_manaRestoringAmount);
    }
}

#undef LOCTEXT_NAMESPACE
