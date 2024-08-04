// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetsPresenterComponent.h"

#include "HUDWidget.h"
#include "InventoryPresenterWidget.h"

TWeakInterfacePtr<IPawnBindableWidget> UWidgetsPresenterComponent::GetHUDWidget() {
    TWeakInterfacePtr<IPawnBindableWidget> asPawnBindable = Cast<IPawnBindableWidget>(_hudWidget);
    check(asPawnBindable.IsValid());
    return asPawnBindable;
}

TWeakInterfacePtr<IInventoryContainerWidget> UWidgetsPresenterComponent::GetInventoryContainerWidget() {
    TWeakInterfacePtr<IInventoryContainerWidget> asInventoryContainer = Cast<IInventoryContainerWidget>(_inventoryPresenterWidget);
    check(asInventoryContainer.IsValid());
    return asInventoryContainer;
}

void UWidgetsPresenterComponent::CreateHUD() {
    const TObjectPtr<APlayerController> ownerPlayerC = Cast<APlayerController>(GetOwner());
    check(IsValid(ownerPlayerC));
    check(IsValid(_hudWidgetClass));

    _hudWidget = CreateWidget<UHUDWidget>(ownerPlayerC.Get(), _hudWidgetClass);
    _hudWidget->SetPawn(ownerPlayerC->GetPawn());
    _hudWidget->BindCurrentPawn();
    _hudWidget->AddToViewport();
}

void UWidgetsPresenterComponent::CreateHUD(const TObjectPtr<USkillsContainerComponent>& skillsContainer) {
    CreateHUD();
    _hudWidget->InitSkillIconContainer(skillsContainer);
}

void UWidgetsPresenterComponent::CreateInventoryMenu() {
    const TObjectPtr<APlayerController> ownerPlayerC = Cast<APlayerController>(GetOwner());
    check(IsValid(ownerPlayerC));
    check(IsValid(_inventoryPresenterWidgetClass));

    _inventoryPresenterWidget = CreateWidget<UInventoryPresenterWidget>(ownerPlayerC.Get(), _inventoryPresenterWidgetClass);
}